#version 420
in vec3 exchPosition;
out vec4 FragColor;

uniform sampler2D textureMap;

const float PI = 3.14159265358;

vec2 PointToTexcoord(vec3 normPoint)
{
	float xzProjectionLength = sqrt(normPoint.x * normPoint.x + normPoint.z * normPoint.z);
	float uCoord = abs(asin(normPoint.z/ xzProjectionLength));
	uCoord = normPoint.x >= 0 ? (PI - uCoord) : uCoord;
	uCoord = normPoint.z<= 0 ? (2*PI - uCoord) : uCoord;
	uCoord /= (2*PI);

	float vCoord = (asin(normPoint.y) + PI/2) / PI;

	return vec2(uCoord, vCoord);
}

void main(void)
{
	vec2 texCoord = PointToTexcoord(exchPosition);
	texCoord.y = 1 - texCoord.y;
   FragColor = texture(textureMap, texCoord);
}
