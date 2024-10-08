#include "viewer.hpp"

#include "../image/image.hpp"

#include <iostream>

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#ifdef _DEBUG
#define GL_CALL(query) do { query; checkOpenGLError(#query, __FILE__, __LINE__); } while(0);
#else
#define GL_CALL(query) query
#endif // _DEBUG

const char* getGLErrorString(GLenum code);
void checkOpenGLError(const char* query, const char* file, int line);

void errorCallback(int errorCode, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseBtnCallback(GLFWwindow* window, int key, int action, int mods);
void scrollCallback(GLFWwindow* window, double dx, double dy);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

int initGLFW(GLFWwindow** window, int w, int h, rt::Viewer& viewer);
int initGLEW();
int initImGui(GLFWwindow* window);

void exitGLFW(GLFWwindow** window);
void exitGLEW();
void exitImGui();

namespace rt {

	Viewer::Viewer(bool enabled, const Image& image, int padding, float updatePerSec):
        _enabled(enabled), _valid(false),
        _ww(0), _wh(0), _pd(padding),
        _image(image), _window(nullptr),
        _updatePerSec(updatePerSec), _accTimeSinceLastUpdateSec(0),
        _imageTex(0)
	{
        _ww = image.width()  + padding * 2;
        _wh = image.height() + padding * 2;
	}

	void Viewer::init() {
        if (!_enabled) return;
        if (int err = initGLFW((GLFWwindow**)&_window, _ww, _wh, *this); err != 0) {
            std::cerr << "Error initializing GLFW, <code> = " << err << "\n";
            _valid = false;
            return;
        }
        if (int err = initGLEW(); err != GLEW_OK) {
            std::cerr << "Error initializing GLEW, <code> = " << glewGetErrorString(err) << "\n";
            _valid = false;
            return;
        }
        if (int err = initImGui((GLFWwindow*)_window); err != 0) {
            std::cerr << "Error initializing ImGui, <code> = " << err << "\n";
            _valid = false;
            return;
        }
        _valid = true;
        //
        GL_CALL(glGenTextures(1, &_imageTex));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, _imageTex));
        GL_CALL(
            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGB,
                _image.width(), _image.height(),
                0, GL_RGB, GL_UNSIGNED_BYTE, NULL
            )
        );
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        float borderColor[] = { 1.0f, 1.0f, 1.0f };
        GL_CALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, NULL));
	}

    void Viewer::exit() {
        if (!_enabled) return;
        exitImGui();
        exitGLEW();
        exitGLFW((GLFWwindow**)&_window);
    }

    void Viewer::start() {
        if (!_enabled) return;
        if (!_valid) return;
        //
        int toWait = floorf(60.0f / _updatePerSec);
        glfwSwapInterval(toWait);
        f64 lastFrameTimeSec = glfwGetTime();
        while (!glfwWindowShouldClose((GLFWwindow*)_window)) {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::SetShortcutRouting(ImGuiMod_Ctrl | ImGuiKey_Tab, ImGuiKeyOwner_None);
            ImGui::SetShortcutRouting(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Tab, ImGuiKeyOwner_None);
            //
            {
                f64 now = glfwGetTime();
                _accTimeSinceLastUpdateSec += (now - lastFrameTimeSec);
                lastFrameTimeSec = now;
                if (_accTimeSinceLastUpdateSec >= (1.0 / _updatePerSec)) {
                    // to ensure no more than 1 update with low-fps
                    _accTimeSinceLastUpdateSec = 0;
                    update();
                }
                preview();
            }
            //
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers((GLFWwindow*)_window);
        }
    }

    void Viewer::stop() {
        if (!_enabled) return;
        glfwSetWindowShouldClose((GLFWwindow*)_window, GLFW_TRUE);
    }

    void Viewer::preview() {
        if (!_enabled) return;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoNav
            | ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_NoInputs
            | ImGuiWindowFlags_NoSavedSettings
            ;
        float progressBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImGui::SetNextWindowPos(ImVec2((float)_pd, (float)_pd - progressBarHeight));
        ImGui::SetNextWindowContentSize(ImVec2((float)_image.width(), (float)_pd));
        ImGui::Begin("progress", nullptr, flags);
        ImGui::ProgressBar(_image.percentage());
        ImGui::End();
        ImGui::SetNextWindowPos(ImVec2((float)_pd, (float)_pd));
        ImGui::SetNextWindowContentSize(ImVec2((float)_image.width(), (float)_image.height()));
        ImGui::Begin("Preview", nullptr, flags);
#pragma warning(push)
#pragma warning(disable: 4312) // 'type cast': conversion from 'unsigned int' to 'ImTextureID' of greater size
        ImGui::Image((ImTextureID)_imageTex, ImVec2((float)_image.width(), (float)_image.height()));
#pragma warning(pop)
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void Viewer::update() {
        if (!_enabled) return;
        GL_CALL(glBindTexture(GL_TEXTURE_2D, _imageTex));
        GL_CALL(
            glTexSubImage2D(
                GL_TEXTURE_2D, 0, 0, 0,
                _image.width(), _image.height(),
                GL_RGB, GL_UNSIGNED_BYTE, _image.rgb()
            )
        );
        GL_CALL(glBindTexture(GL_TEXTURE_2D, NULL));
    }

} // namespace rt


// ==========================================================================================
// GLFW
// ==========================================================================================

int initGLFW(GLFWwindow** window, int w, int h, rt::Viewer& viewer) {
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 4.5+ only
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    (*window) = glfwCreateWindow(w, h, "Ray Tracer", NULL, NULL);
    if (!*window) return 2;
    glfwSetWindowUserPointer(*window, &viewer);
    glfwSetKeyCallback(*window, keyCallback);
    glfwSetMouseButtonCallback(*window, mouseBtnCallback);
    glfwSetScrollCallback(*window, scrollCallback);
    glfwSetFramebufferSizeCallback(*window, framebufferSizeCallback);
    glfwMakeContextCurrent(*window);
    return 0;
}

void exitGLFW(GLFWwindow** window) {
    glfwDestroyWindow(*window);
    glfwTerminate();
}

void errorCallback(int errorCode, const char* description) {
    std::cerr << "GLFW error, <code> = " << errorCode << " : " << description << "\n";
    exit(errorCode);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //
}

void mouseBtnCallback(GLFWwindow* window, int key, int action, int mods) {
    //
}

void scrollCallback(GLFWwindow* window, double dx, double dy) {
    //
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    //
}

// ==========================================================================================
// GLEW
// ==========================================================================================

int initGLEW() {
    GLenum err = glewInit();
    if (GLEW_OK != err) return err;
    return GLEW_OK;
}

void exitGLEW() {
    //
}

// ==========================================================================================
// IMGUI
// ==========================================================================================

int initImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) return 1;
    if (!ImGui_ImplOpenGL3_Init("#version 450")) return 2;
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr; // disable imgui.ini
    io.LogFilename = nullptr; // disable log
    ImFontConfig cfg;
    cfg.SizePixels = 22;
    io.Fonts->AddFontDefault(&cfg);
    return 0;
}

void exitImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// ==========================================================================================
// OpenGL
// ==========================================================================================

#define CASE_STR(value) case value: return #value;
const char* getGLErrorString(GLenum code) {
    switch (code) {
        CASE_STR(GL_INVALID_ENUM)
        CASE_STR(GL_INVALID_VALUE)
        CASE_STR(GL_INVALID_OPERATION)
        CASE_STR(GL_INVALID_FRAMEBUFFER_OPERATION)
        CASE_STR(GL_OUT_OF_MEMORY)
        CASE_STR(GL_STACK_UNDERFLOW)
        CASE_STR(GL_STACK_OVERFLOW)
        CASE_STR(GL_NO_ERROR)
        default: return "<unknown>";
    }
}
#undef CASE_STR

void checkOpenGLError(const char* query, const char* file, int line) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("OpenGL error %s (%08x)\nAt %s:%i\n\"%s\"\n", getGLErrorString(error), error, file, line, query);
        exit(error);
    }
}
