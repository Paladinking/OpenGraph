#pragma once
#include <vector>
#include <string>

struct Shader {
	const char* path;
	const unsigned int type;
};

class ShaderProgram {
public:
	unsigned int id;

	ShaderProgram(std::vector<Shader> shaders);

	void use() const;

	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;

	void setFloat(const std::string& name, float value) const;

	void setFloat(const std::string& name, float f1, float f2) const;

	void setFloat(const std::string& name, float f1, float f2, float f3) const;

	void setFloat(const std::string& name, float f1, float f2, float f3, float f4) const;

};

