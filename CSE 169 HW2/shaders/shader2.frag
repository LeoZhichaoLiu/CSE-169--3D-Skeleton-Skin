#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 outputColor;
in vec3 FragPos;  //The position of fragment.
in vec3 Normal;   // The color of that fragment.

uniform vec3 color;

uniform bool only_point;
uniform bool only_spot;

// Material values:
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

//Point light value: light_color/position.
uniform vec3 light_color;
uniform vec3 LightPos;

//Spit light value: light_color/position/Cutoff/Angle.
uniform vec3 spot_color;
uniform vec3 SpotPos;
uniform float cutoff;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
       vec3 light = vec3 (0, 0, 0);
    
       
       if (only_point) {
      
         vec3 Ambient = light_color * ambient;
      
         vec3 normal = normalize(Normal);
         vec3 light_direction = normalize (LightPos - FragPos);
         vec3 Diffuse =  light_color * (max (dot(normal, light_direction), 0)) * diffuse;
    
         vec3 view_direction = normalize (vec3(0,0,1) - FragPos);
         vec3 reflect_direction = reflect(-light_direction, normal);
         float specular_factor = pow(max (dot(view_direction, reflect_direction), 0), shininess);
         vec3 Specular = light_color * specular_factor * specular;
    
         // Attenuation
         float distance = length(LightPos - FragPos);
         float attenuation = 1.0f / (0.09 * distance);
    
         light = attenuation * (Ambient + Diffuse + Specular);
       }
    
       if (only_spot) {
         vec3 Specular2 = vec3 (0, 0, 0);
         float intensity = 0;
       
         vec3 Ambient2 = spot_color * ambient;
      
         vec3 normal2 = normalize(Normal);
         vec3 light_direction2 = normalize (SpotPos - FragPos);
         vec3 Diffuse2 = spot_color * diffuse;
       
         vec3 Spot_direction = normalize (SpotPos - vec3 (0, 0, 0));
      
      
         if (dot (light_direction2, Spot_direction) > cutoff || dot (light_direction2, Spot_direction) < -1 * cutoff ) {
            vec3 view_direction2 = normalize (vec3(0,0,1) - FragPos);
            vec3 reflect_direction2 = reflect(-light_direction2, normal2);
            float specular_factor2 = pow(max (dot(view_direction2, reflect_direction2), 0), shininess);
            Specular2 = spot_color * specular_factor2 * specular;
            intensity = (max (dot(normal2, light_direction2), 0));
         }
    
        float distance2 = length(SpotPos - FragPos);
        float attenuation2 = 1.0f / (0.09 * distance2 /*+ 0.032 * distance2 * distance2*/);
      
        light = light + attenuation2 * (Ambient2 + intensity * Diffuse2 + Specular2);
      }
        
      fragColor = vec4(light, sampleExtraOutput);
        
}
