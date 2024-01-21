#include "xhpch.h"
#include "OpenGLShader.h"

#include "XH/Core/Time.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

// #include <shaderc/shaderc.hpp>
// #include <spirv_cross/spirv_cross.hpp>
// #include <spirv_cross/spirv_glsl.hpp>

namespace XH
{
    namespace Utils 
    {

        static GLenum ShaderTypeFromString(const std::string& type)
        {
            if (type == "vertex")
                return GL_VERTEX_SHADER;
            if (type == "fragment" || type == "pixel")
                return GL_FRAGMENT_SHADER;

            XH_CORE_ASSERT(false, "Unknown shader type!");
            return 0;
        }
        /*
        static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
                case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
            }
            XH_CORE_ASSERT(false);
            return (shaderc_shader_kind)0;
        }
        */

        static const char* GLShaderStageToString(GLenum stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
                case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
            }
            XH_CORE_ASSERT(false);
            return nullptr;
        }

        static const char* GetCacheDirectory()
        {
            // TODO: make sure the assets directory is valid
            return "assets/cache/shader/opengl";
        }
        /*
        static void CreateCacheDirectoryIfNeeded()
        {
            std::string cacheDirectory = GetCacheDirectory();
            if (!std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }
        */

        static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
                case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
            }
            XH_CORE_ASSERT(false);
            return "";
        }

        static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
                case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
            }
            XH_CORE_ASSERT(false);
            return "";
        }
    }

    OpenGLShader::OpenGLShader(const std::string& path)
        :m_FilePath(path)
    {
        XH_PROFILE_FUNCTION();

        std::string sourceStr = ReadFile(path);
        auto shaderSources = PreProcess(sourceStr);
        CreateProgram(shaderSources);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        :m_Name(name)
    {
        XH_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        CreateProgram(sources);
    }

    OpenGLShader::~OpenGLShader()
    {
        XH_PROFILE_FUNCTION();

        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const
    {
        XH_PROFILE_FUNCTION();

        glUseProgram(m_RendererID);
    }

    void OpenGLShader::UnBind() const
    {
        XH_PROFILE_FUNCTION();

        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        XH_PROFILE_FUNCTION();

        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
        XH_PROFILE_FUNCTION();

        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        XH_PROFILE_FUNCTION();

        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
    {
        XH_PROFILE_FUNCTION();

        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        XH_PROFILE_FUNCTION();

        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        XH_PROFILE_FUNCTION();

        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        XH_PROFILE_FUNCTION();

        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        XH_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
            else
            {
                XH_CORE_ERROR("Could not read from File: {0}", filepath);
            }
        }
        else
        {
            XH_CORE_ERROR("Could not Open File: {0}", filepath);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        XH_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            XH_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);
            XH_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            XH_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

            shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::CreateProgram(std::unordered_map<GLenum, std::string> shaderSources)
    {
        XH_PROFILE_FUNCTION();

        GLuint program = glCreateProgram();
        std::vector<GLuint> shaderIDs;

        for (const auto& [type, shader]:shaderSources)
        {
            GLuint shaderId = glCreateShader(type);
            shaderIDs.push_back(shaderId);
            const GLchar* source = static_cast<const GLchar*>(shader.c_str());
            glShaderSource(shaderId, 1, &source, 0);
            glCompileShader(shaderId);

            GLint isCompiled = 0;
            glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shaderId);

                XH_CORE_ERROR("{0}", infoLog.data());
                XH_CORE_ASSERT(false, type + "Shader compilation failure!");
                return;
            }
        }
      

        m_RendererID = program;
        for(auto id :shaderIDs)
            glAttachShader(program, id);
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, static_cast<int*>(&isLinked));
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);
            for (auto id : shaderIDs)
                glDeleteShader(id);
            XH_CORE_ERROR("{0}", infoLog.data());
            XH_CORE_ASSERT(false, "Shader Link failure!");
            return;
        }

        for (auto id : shaderIDs)
            glDetachShader(program, id);
    }
}