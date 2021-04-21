#version 330

uniform mat4 modelMatrix;
uniform vec3 cameraPosition;

uniform sampler2D textureSplatMap;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureDiffuse2;
uniform sampler2D textureDiffuse3;
uniform sampler2D textureDiffuse4;

uniform float materialShininess;
uniform vec3 materialSpecularColor;

#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light {
   vec4 position;
   vec3 intensities; //a.k.a the color of the light
   float attenuation;
   float ambientCoefficient;
   float coneAngle;
   vec3 coneDirection;
} allLights[MAX_LIGHTS];

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 finalColor;

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {
    vec3 surfaceToLight;
    float attenuation = 1.0;
    if(light.position.w == 0.0) {
        //directional light
        surfaceToLight = normalize(light.position.xyz);
        attenuation = 1.0; //no attenuation for directional lights
    } else {
        //point light
        surfaceToLight = normalize(light.position.xyz - surfacePos);
        float distanceToLight = length(light.position.xyz - surfacePos);
        attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

        //cone restrictions (affects attenuation)
        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
        if(lightToSurfaceAngle > light.coneAngle){
            attenuation = 0.0;
        }
    }

    //ambient
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;
    
    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

    //linear color (color before gamma correction)
    return ambient + attenuation*(diffuse + specular);
}

void main() {
    vec3 normal = normalize(transpose(inverse(mat3(modelMatrix))) * fragNormal);
    vec3 surfacePos = vec3(modelMatrix * vec4(fragVert, 1));
    
    vec4 splatMapColor = texture(textureSplatMap, fragTexCoord);

    vec4 diffuse1 = texture(textureDiffuse1, fragTexCoord);
    vec4 diffuse2 = texture(textureDiffuse2, fragTexCoord);
    vec4 diffuse3 = texture(textureDiffuse3, fragTexCoord);
    vec4 diffuse4 = texture(textureDiffuse4, fragTexCoord);

    diffuse1 *= splatMapColor.x;
    diffuse2 *= splatMapColor.y;
    //diffuse3 *= splatMapColor.z;

    vec4 surfaceColor = diffuse1 + diffuse2 + diffuse3;

    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

    //combine color from all the lights
    vec3 linearColor = vec3(0);
    for(int i = 0; i < numLights; ++i){
        linearColor += ApplyLight(allLights[i], surfaceColor.rgb, normal, surfacePos, surfaceToCamera);
    }
    
    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}

//void main() {
//    //calculate normal in world coordinates
//    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
//    vec3 normal = normalize(normalMatrix * fragNormal);
//    
//    //calculate the location of this fragment (pixel) in world coordinates
//    vec3 fragPosition = vec3(modelMatrix * vec4(fragVert, 1));
//    
//    vec4 surfaceColor = texture(diffuseTexture, fragTexCoord);
//
//    //combine color from all the lights
//    for(int i = 0; i < numLights; ++i){
//        vec3 surfaceToLight = allLights[i].position.xyz - fragPosition;
//
//        //calculate the cosine of the angle of incidence
//        float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
//        brightness = clamp(brightness, 0, 1);
//
//        //calculate final color of the pixel, based on:
//        // 1. The angle of incidence: brightness
//        // 2. The color/intensities of the light: light.intensities
//        // 3. The texture and texture coord: texture(tex, fragTexCoord)
//
//        finalColor += vec4(brightness * allLights[0].intensities * surfaceColor.rgb, surfaceColor.a);
//    }
//}