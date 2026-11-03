#include "render/ShaderProgram.hpp"

#include <format>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

std::string read_file(const std::filesystem::path& path)
{
    std::ifstream file{path, std::ios::binary};
    if (!file) {
        throw std::runtime_error(std::format("couldn't open shader file: {}", path.string()));
    }
    // Read the whole file: iterate from the first character to "end of stream" ({}).
    return std::string{std::istreambuf_iterator<char>{file}, {}};
}

// The driver's info log ends with a '\0' and usually a newline; trim both.
std::string trim_log(std::string log)
{
    while (!log.empty() && (log.back() == '\0' || log.back() == '\n')) {
        log.pop_back();
    }
    return log;
}

std::string shader_log(GLuint shader)
{
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length <= 0) {
        return {};
    }
    std::string log(static_cast<std::size_t>(length), '\0');
    glGetShaderInfoLog(shader, length, nullptr, log.data());
    return trim_log(std::move(log));
}

std::string program_log(GLuint program)
{
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length <= 0) {
        return {};
    }
    std::string log(static_cast<std::size_t>(length), '\0');
    glGetProgramInfoLog(program, length, nullptr, log.data());
    return trim_log(std::move(log));
}

// One compiled shader stage (vertex or fragment) (RAII). Only needed while the
// program links, so it lives here rather than in the header.
class ShaderStage {
public:
    ShaderStage(GLenum stage, const std::filesystem::path& path)
    {
        const std::string source = read_file(path);   // before creating anything: nothing to clean up if it throws
        m_id = glCreateShader(stage);

        const char* text = source.c_str();
        glShaderSource(m_id, 1, &text, nullptr);
        glCompileShader(m_id);

        GLint compiled = GL_FALSE;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &compiled);
        if (compiled != GL_TRUE) {
            const std::string log = shader_log(m_id);
            // A constructor that throws never finished, so the destructor
            // will NOT run. Clean up by hand before throwing.
            glDeleteShader(m_id);
            throw std::runtime_error(
                std::format("{} failed to compile:\n{}", path.filename().string(), log));
        }
    }

    ~ShaderStage() { glDeleteShader(m_id); }

    ShaderStage(const ShaderStage&) = delete;
    ShaderStage& operator=(const ShaderStage&) = delete;
    ShaderStage(ShaderStage&&) = delete;
    ShaderStage& operator=(ShaderStage&&) = delete;

    GLuint id() const { return m_id; }

private:
    GLuint m_id = 0;
};

} // namespace

ShaderProgram::ShaderProgram(const std::filesystem::path& vertex_path,
                             const std::filesystem::path& fragment_path)
{
    // If the fragment shader fails, `vertex` is already fully built, so its
    // destructor runs during unwinding and nothing leaks.
    const ShaderStage vertex{GL_VERTEX_SHADER, vertex_path};
    const ShaderStage fragment{GL_FRAGMENT_SHADER, fragment_path};

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex.id());
    glAttachShader(m_id, fragment.id());
    glLinkProgram(m_id);

    GLint linked = GL_FALSE;
    glGetProgramiv(m_id, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        const std::string log = program_log(m_id);
        glDeleteProgram(m_id);          // our destructor won't run (see ShaderStage)
        throw std::runtime_error(std::format("shaders {} + {} failed to link:\n{}",
                                             vertex_path.filename().string(),
                                             fragment_path.filename().string(), log));
    }

    // The linked program keeps its own copy. Detach so the stages can be
    // freed when `vertex` and `fragment` go out of scope just below.
    glDetachShader(m_id, vertex.id());
    glDetachShader(m_id, fragment.id());
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_id);              // 0 (moved from) is silently ignored
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : m_id{std::exchange(other.m_id, 0)}
{
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other) {
        glDeleteProgram(m_id);
        m_id = std::exchange(other.m_id, 0);
    }
    return *this;
}

void ShaderProgram::use() const
{
    glUseProgram(m_id);
}
