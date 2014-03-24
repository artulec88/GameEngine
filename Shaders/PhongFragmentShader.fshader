#version 330 core

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
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

uniform sampler2D sampler;
uniform vec3 eyePos; // camera position
uniform vec3 baseColor;
uniform vec3 ambientLight;
uniform float specularIntensity;
uniform float specularPower;
uniform DirectionalLight directionalLight; // maybe create an array so that multiple directional lights can be handled
uniform PointLight pointLights[MAX_POINT_LIGHTS]; // GLSL does not work well with variable-length tables, so do not use some uniform as the length of the table
uniform SpotLight spotLights[MAX_SPOT_LIGHTS]; // GLSL does not work well with variable-length tables, so do not use some uniform as the length of the table

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

vec4 CalcDirectionalLight(DirectionalLight directionalLight, vec3 normal)
{
	return CalcLight(directionalLight.base, -directionalLight.direction, normal);
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
	vec4 totalLight = vec4(ambientLight, 1.0);
	vec4 textureColor = texture2D(sampler, texCoord0.xy);
	vec4 color = vec4(baseColor, 1.0);
	
	if (textureColor != vec4(0.0, 0.0, 0.0, 0.0))
	{
		color *= textureColor;
	}
	
	vec3 normal = normalize(normal0); // in case normal0 was not delivered as a normalized vector
	
	totalLight += CalcDirectionalLight(directionalLight, normal);
	
	vec4 totalLightFromPointLights = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if(pointLights[i].base.intensity > 0)
		{
			totalLightFromPointLights += CalcPointLight(pointLights[i], normal);
		}
	}
	totalLight += totalLightFromPointLights;
	
	vec4 totalLightFromSpotLights = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < MAX_SPOT_LIGHTS; ++i)
	{
		if(spotLights[i].pointLight.base.intensity > 0)
		{
			totalLightFromSpotLights += CalcSpotLight(spotLights[i], normal);
		}
	}
	totalLight += totalLightFromSpotLights;
	
	fragColor = color * totalLight; // comment this out once you stop debugging
}