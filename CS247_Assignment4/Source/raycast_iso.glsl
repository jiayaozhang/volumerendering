// textures
uniform sampler1D transferfunction;
uniform sampler3D volume_tex;
uniform sampler2D backface_tex;
uniform sampler2D frontface_tex;
uniform float step_size;
uniform float ambient;

uniform float iso_value;

// For Shading
vec3 I_ambient = vec3(ambient);
vec3 I_diffuse = vec3(0.5);
vec3 I_specular = vec3(0.5);
float shininess = 32.0;

// For opacity correction
float base_step_size = 0.0025f;

// parametes
uniform vec4 params;


void main()
{
	//debug output: a single color
	gl_FragColor = params;
	
	//TODO: to iso surface raycasting here
	vec4 value;
	float scalar;

	// Initialize accumulated color and opacity 
	vec4 dst = vec4(0, 0, 0, 0);

	// Find the ray start and stop vertices (bounding), in texture dim, [0, 1]
	vec3 volExtentMin = vec3(texture2D(frontface_tex, gl_TexCoord[0].xy)); // The texture/color value is the coordinate to 3D volume
	vec3 volExtentMax = vec3(texture2D(backface_tex, gl_TexCoord[0].xy));

	// Determine volume entry position 
	vec3 position = volExtentMin; 
	// Compute ray direction 
	vec3 direction = normalize(volExtentMax - volExtentMin);

	// Loop for ray traversal
	float max_marching_steps = length(volExtentMax - volExtentMin) / step_size; // For exiting
	for (int i = 0; i < max_marching_steps; i++) 
	{ 
		// Data access to scalar value in 3D volume texture 
		value = texture3D(volume_tex, position);
		// Why take .a here? What are other components?
		scalar = value.a;

		if (scalar >= iso_value)
		{

			// Do all the same as dvr for this point only
			vec4 src = texture1D(transferfunction, scalar); 
			float opacity = src.a;
			opacity = 1 - pow((1 - opacity), step_size / base_step_size);
			vec4 src_new = vec4(src.rgb * opacity, opacity);

			// calcutlate norm
			vec3 norm;
			float delta = step_size;
			norm.x = texture3D(volume_tex, position + vec3(delta, 0.0, 0.0)).x - texture3D(volume_tex, position - vec3(delta, 0.0, 0.0)).x;
			norm.y = texture3D(volume_tex, position + vec3(0.0, delta, 0.0)).x - texture3D(volume_tex, position - vec3(0.0, delta, 0.0)).x;
			norm.z = texture3D(volume_tex, position + vec3(0.0, 0.0, delta)).x - texture3D(volume_tex, position - vec3(0.0, 0.0, delta)).x;
			norm = normalize(norm);

			// calculate diffuse and specular
			float RayDotNorm = dot(direction, norm);
			vec3 diffuse = max(RayDotNorm, 0.0) * I_diffuse;
			vec3 specular;
			if (RayDotNorm > 0.0)
			{
				vec3 v = normalize(-position.xyz);
				vec3 r = reflect(-direction, norm);
				vec3 spec = I_specular * pow(max(dot(r, v), 0.0), shininess);
			}

			vec4 final_color = vec4(I_ambient + diffuse + specular, 1.0) * src_new * 3;
			dst = (1.0 - dst.a) * final_color + dst;

			break; // Early ray termination
		}

		position = position + direction * step_size;

		// Early ray termination
		// This is actually not working, as there is no accumulation of opacity
		if(dst.a > 0.95)
			break;

	} 


	

	gl_FragColor = dst;
}
