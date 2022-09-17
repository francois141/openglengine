vec4 computeLightPosition(mat4 lightSpaceMatrix, mat4 modelMatrix, vec3 position) {
	return lightSpaceMatrix * modelMatrix * vec4(position,1.0);
}

vec4 computeWorldPosition(mat4 projectionMatrix, mat4 viewMatrix, mat4 modelMatrix, vec3 position) {
	return projectionMatrix * viewMatrix * modelMatrix * vec4(position,1.0);
}

vec4 computePosition(mat4 lightSpaceMatrix, mat4 projectionMatrix, mat4 viewMatrix, mat4 modelMatrix, vec3 position, int RenderType) {
    return RenderType == SHADOW ? computeLightPosition(lightSpaceMatrix,modelMatrix,position) : computeWorldPosition(projectionMatrix,viewMatrix,modelMatrix,position);
}
