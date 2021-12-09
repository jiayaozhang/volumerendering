// textures
uniform sampler1D transferfunction;
uniform sampler3D volume_tex;
uniform sampler2D backface_tex;
uniform sampler2D frontface_tex;
uniform float step_size;
uniform float ambient;
uniform bool MIP_switch;

uniform vec3 vol_dim;
uniform int vol_max_dim;

// For Shading
vec3 I_ambient = vec3(ambient);
vec3 I_diffuse = vec3(0.5);
vec3 I_specular = vec3(0.5);
float shininess = 32.0;

// For opacity correction
float base_step_size = 0.0025f;

void main()
{

	//debug output: tranfer function color is displayed for current fragment's x coordinate
	//gl_FragColor = texture1D ( transferfunction, gl_TexCoord[0].x );
	
	//TODO: implement DVR here
	// Following the book Real-Time Volume Graphics

	vec4 value;
	float scalar;
	float MIP = 0.0;
	vec4 MIP_color;

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

		// Apply transfer function 
		vec4 src = texture1D(transferfunction, scalar); 
		float opacity = src.a;

		// Opacity correction
		// https://stackoverflow.com/questions/12494439/opacity-correction-in-raycasting-volume-rendering
		//float volume_resolution_rate_on_ray = 1 / abs(dot(direction, vol_dim) * vol_max_dim); // TODO debug when dot product == 0
		//float correction_ratio = step_size / volume_resolution_rate_on_ray;
		opacity = 1 - pow((1 - opacity), step_size / base_step_size);
		vec4 src_new = vec4(src.rgb * opacity, opacity);
		//vec4 src_new = vec4(src.rgb * src.a, src.a); // Opacity Uncorrected

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
			vec3 specular = I_specular * pow(max(dot(r, v), 0.0), shininess);
		}
		vec4 final_color = vec4(I_ambient + diffuse + specular, 1.0) * src_new;

		if (scalar > MIP)
		{
			MIP = scalar;
			MIP_color = final_color;
		}

		// Front-to-back compositing 
		// Color and opacity 
		dst = (1.0 - dst.a) * final_color + dst;

		// Advance ray position along ray direction 
		position = position + direction * step_size;

		//// Ray termination: Test if outside volume ...
		//// This is from the textbook, as the max_marching_steps is used, no need for this
		//vec3 temp1 = sign(position - volExtentMin); 
		//vec3 temp2 = sign(volExtentMax - position); 
		//float inside = dot(temp1, temp2); 
		//// ... and exit loop 
		//if (inside < 3.0) 
		//	break;

		// Early ray termination
		if(dst.a > 0.95)
			break;

	} 

	

	if (MIP_switch == false) 
		gl_FragColor = dst;
	else
		gl_FragColor = MIP_color;
}
