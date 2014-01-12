#version 330
uniform vec3 camPos;
uniform sampler2D tex;
uniform vec4 Planets[16];
uniform float PlanetRadii[16];
uniform float GravConstant;
uniform float MaxGFS;

#define PI 3.1415926535897932384626433832795

varying vec4 worldCoord;
out vec4 FragColor;

vec4 Colour()
{
	vec4 color;
	vec3 a;
	a.x = 0;
	a.y = 0;
	a.z = 0;
	vec3 pos = worldCoord.xyz + camPos; //
	
	float mag = 0.0;
	for(int i = 0; i < Planets.length(); i++)
	{
		vec3 gDir = normalize(Planets[i].xyz - pos);
		mag = length(Planets[i].xyz - pos);
		if(mag < PlanetRadii[i])
			a += gDir * (GravConstant * Planets[i].w / (PlanetRadii[i] * PlanetRadii[i])) * sin(0.5*PI*mag/PlanetRadii[i]);
		else
			a += gDir * (GravConstant * Planets[i].w / (mag*mag));
	}
	
	mag = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	if(mag > MaxGFS)
		color = vec4(1.0, 1.0, 1.0, 1.0);
	else
		color = texture(tex, vec2(1 - (mag/MaxGFS), 1.0));
	
	return color;
}

void main ()
{
	FragColor = Colour();
}