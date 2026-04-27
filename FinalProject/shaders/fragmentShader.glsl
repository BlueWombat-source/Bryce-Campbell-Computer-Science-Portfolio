#version 440 core

struct Material 
{
    vec3 ambientColor;
    float ambientStrength;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
}; 

struct LightSource 
{
    vec3 position;	
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float focalStrength;
    float specularIntensity;
    
    float constant;
    float linear;
    float quadratic;
};

#define TOTAL_LIGHTS 4

in vec3 fragmentPosition;
in vec3 fragmentVertexNormal;
in vec2 fragmentTextureCoordinate;

out vec4 outFragmentColor;

uniform bool bUseTexture=false;
uniform bool bUseLighting=false;
uniform vec4 objectColor = vec4(1.0f);
uniform sampler2D objectTexture;
uniform vec3 viewPosition;
uniform vec2 UVscale = vec2(1.0f, 1.0f);
uniform LightSource lightSources[TOTAL_LIGHTS];
uniform Material material;
uniform float alphaMultiplier = 1.0;

// function prototypes
vec3 CalcLightSource(LightSource light, vec3 lightNormal, vec3 vertexPosition, vec3 viewDirection);

void main()
{
   if(bUseLighting == true)
   {
      // properties
      vec3 lightNormal = normalize(fragmentVertexNormal);
      vec3 viewDirection = normalize(viewPosition - fragmentPosition);
      vec3 phongResult = vec3(0.0f);

      for(int i = 0; i < TOTAL_LIGHTS; i++)
      {
         phongResult += CalcLightSource(lightSources[i], lightNormal, fragmentPosition, viewDirection); 
      }   
    
      if(bUseTexture == true)
      {
         vec4 textureColor = texture(objectTexture, fragmentTextureCoordinate * UVscale);
         outFragmentColor = vec4(phongResult * textureColor.xyz, 1.0 * alphaMultiplier);
      }
      else
      {
         outFragmentColor = vec4(phongResult * objectColor.xyz, objectColor.w);
      }
   }
   else 
   {
      if(bUseTexture == true)
      {
         vec4 texColor = texture(objectTexture, fragmentTextureCoordinate * UVscale);
         outFragmentColor = vec4(texColor.rgb, texColor.a * alphaMultiplier);
      }
      else
      {
         outFragmentColor = objectColor;
      }
   }
}

// calculates the color when using a directional light.
vec3 CalcLightSource(LightSource light, vec3 lightNormal, vec3 vertexPosition, vec3 viewDirection)
{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   //**Calculate Ambient lighting**

   // changed from: ambient = light.ambientColor + material.ambientColor * material.ambientStrength;
   // blends color from light and material rather than adding, which led to white appearance
   ambient = light.ambientColor * material.ambientColor * material.ambientStrength;

   //**Calculate Diffuse lighting**

   // Calculate distance (light direction) between light source and fragments/pixels
   vec3 lightDirection = normalize(light.position - vertexPosition);

   float NdotL = dot(lightNormal, lightDirection);

   float diffuseFactor = max(NdotL, 0.0);
   float transmissionFactor = max(-NdotL, 0.0);

   // combine diffuse and transmission factors
   float lightingFactor = diffuseFactor + transmissionFactor * 0.6;

   // apply to diffuse
   diffuse = light.diffuseColor * lightingFactor * material.diffuseColor;

   //**Calculate Specular lighting**

   // Calculate reflection vector
   vec3 reflectDir = reflect(-lightDirection, lightNormal);
   // Calculate specular component
   float specularComponent = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
   // changed from: specular = (light.specularIntensity * material.shininess) * specularComponent * material.specularColor;
   // this change reduces the wash out from multiplying by shininess
   specular = light.specularColor * light.specularIntensity * specularComponent * material.specularColor;

   //**Calculate Attentuation**
   float distance = length(light.position - fragmentPosition);
   float denom = (light.constant + light.linear * distance + light.quadratic * (distance * distance));
   float attenuation = 1.0 / max(denom, 0.001);

   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;
  
   // clamp final value to ensure it does not exceed 1.0
   return clamp(ambient + diffuse + specular, 0.0, 1.0);
}