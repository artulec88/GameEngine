function calculate(input)
{
	var m00 = parseFloat(input.m00.value);
	var m01 = parseFloat(input.m01.value);
	var m02 = parseFloat(input.m02.value);
	var m10 = parseFloat(input.m10.value);
	var m11 = parseFloat(input.m11.value);
	var m12 = parseFloat(input.m12.value);
	var m20 = parseFloat(input.m20.value);
	var m21 = parseFloat(input.m21.value);
	var m22 = parseFloat(input.m22.value);
	var qw=0;
	var qx=1;
	var qy=0;
	var qz=0;
	var S=0;
	if (m00 + m11 + m22 > 2.9999) { // check for identity matrix
		qw = 1;
		qx = 0;
		qy = 0;
		qz = 0;
	} else if( (m00 + m11 + m22 +1)> 0.0001 ) {
		S = Math.sqrt(m00 + m11 + m22+1)*2;
		qw = 0.25* S;
		qx = ( m21 - m12) /S;
		qy = ( m02 - m20 ) /S;
		qz = ( m10 - m01 ) /S;
	} else if ((m00 > m11)&(m00 > m22)) {
		if (( 1.0 + m00 - m11 - m22 ) <= 0) {
			input.error.value="sqrt("+( 1.0 + m00 - m11 - m22 )+")";
			return;
		}
	
	S = Math.sqrt( 1.0 + m00 - m11 - m22 ) * 2; // S=4*qx
	qw = (m21 - m12) / S;
	qx = 0.25 * S;
	qy = (m01 + m10) / S;
	qz = (m02 + m20) / S;
	} else if (m11 > m22) {
		if (( 1.0 + m11 - m00 - m22 ) <= 0) {
			input.error.value="sqrt("+( 1.0 + m11 - m00 - m22)+")";
			return;
		}
		S = Math.sqrt( 1.0 + m11 - m00 - m22 ) * 2; // S=4*qy
		qw = (m02 - m20) / S;
		qx = (m01 + m10) / S;
		qy = 0.25 * S;
		qz = (m12 + m21) / S;
	} else {
		if (( 1.0 + m22 - m00 - m11 ) <= 0) {
			input.error.value="sqrt("+( 1.0 + m22 - m00 - m11 )+")";
			return;
		}
		S = Math.sqrt( 1.0 + m22 - m00 - m11 ) * 2; // S=4*qz
		qw = (m10 - m01) / S;
		qx = (m02 + m20) / S;
		qy = (m12 + m21) / S;
		qz = 0.25 * S;
	}
	input.qw.value=qw;
	input.qx.value=qx;
	input.qy.value=qy;
	input.qz.value=qz;
	input.error.value=" ";
}