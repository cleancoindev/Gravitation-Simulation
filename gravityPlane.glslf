#version 420
uniform sampler2D textureMap;
uniform vec4 Planets[16];
uniform int PlanetsLen;
uniform float PlanetRadii[16];
uniform float MaxGFS;
uniform float G;

#define PI 3.1415926535897932384626433832795

in vec4 worldCoord;
out vec4 FragColor;

vec4 Colour()
{
	vec4 color;
	vec3 a = vec3(0);
	vec3 pos = worldCoord.xyz;

	float mag = 0.0;
	for(int i = 0; i < PlanetsLen; i++)
	{
		vec3 gDir = normalize(Planets[i].xyz - pos);
		mag = length(Planets[i].xyz - pos);
		if(mag < PlanetRadii[i])
			a += gDir * (Planets[i].w / (PlanetRadii[i] * PlanetRadii[i])) * (mag/PlanetRadii[i]);
		else
			a += gDir * (Planets[i].w / (mag*mag));
	}
	a *= G;

	mag = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	if(mag > MaxGFS)
		color = vec4(1.0, 1.0, 1.0, 1.0);
	else
		color = texture(textureMap, vec2(sqrt(mag/MaxGFS), 1.0));

	return color;
}

void main ()
{
	FragColor = Colour();
}
