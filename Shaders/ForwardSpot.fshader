#version 330 core

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

struct BaseLight
{
	vec3 color;
	float intensity;
};

// defines how quickly the point light fades off with the raising distance between source and target
struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};

struct PointLight
{
	BaseLight base;
	Attenuation attenuation;
	vec3 position;
	float range; // max distance between the source and the point for the light to affect that point
};

struct SpotLight
{
	PointLight pointLight;
	vec3 direction; // general direction of the light
	float cutoff; // defines the cone of the light. The value must be from the interval (0; 1).
};

uniform sampler2D diffuseSampler;
uniform vec3 eyePos; // camera position
uniform float specularIntensity;
uniform float specularPower;
uniform SpotLight spotLight;

vec4 CalcLight(BaseLight base, vec3 direction, vec3 normal)
{
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	float diffuseFactor = dot(normal, -direction); // attenuation
	if (diffuseFactor > 0.0)
	{
		diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
		
		// specular reflection calculation begin
		vec3 directionToEye = normalize(eyePos - worldPos0);
		vec3 reflectDirection = normalize(reflect(direction, normal));
		// compare two vectors to find the specular
		float specularFactor = dot(directionToEye, reflectDirection); // 1 if directionToEye = reflectDirection
		specularFactor = pow(specularFactor, specularPower); // TODO: consider moving this inside "if" condition below
		
		if (specularFactor > 0)
		{
			specularColor = vec4(base.color, 1.0) * specularIntensity * specularFactor;
		}
		// specular reflection calculation end
	}
	
	return diffuseColor + specularColor;
}

vec4 CalcPointLight(PointLight pointLight, vec3 normal)
{
	vec3 lightDirection = worldPos0 - pointLight.position;
	float distanceToLightSource = length(lightDirection);
	if (distanceToLightSource > pointLight.range)
	{
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
	
	lightDirection = normalize(lightDirection);
	
	vec4 color = CalcLight(pointLight.base, lightDirection, normal);
	
	float atten = pointLight.attenuation.constant +
					pointLight.attenuation.linear * distanceToLightSource +
					pointLight.attenuation.exponent * distanceToLightSource * distanceToLightSource +
					0.00001; // just a small number to prevent from dividing by zero
	
	return color / atten; // the bigger the attenuation the smaller the color. TODO: Watch out for attenuation = 0!
}

vec4 CalcSpotLight(SpotLight spotLight, vec3 normal)
{
	vec3 lightDirection = normalize(worldPos0 - spotLight.pointLight.position); // what direction the light is facing
	float spotFactor = dot(lightDirection, spotLight.direction);
	
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	
	if (spotFactor > spotLight.cutoff) // pixel is within cone of light
	{
		color = CalcPointLight(spotLight.pointLight, normal);
		color *= 1.0 - ((1.0 - spotFactor) / (1.0 - spotLight.cutoff)); // when pixel is close to the edge of the cone the light should fade out
	}
	
	return color;
}

void main()
{
	fragColor = texture2D(diffuseSampler, texCoord0.xy) *
		CalcSpotLight(spotLight, normalize(normal0));
}
