#include "ui/Gui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuiFileDialog.h>

#include "ui/JetBrainsMono.h"
#include "core/App.hpp"
#include "core/Constants.hpp"
#include "scene/Node.hpp"
#include "state/AppState.hpp"

namespace snd3D {

    Gui::Gui(App& app, float fontSize) : app(app) {
        this->fontSize = fontSize;

        // Init ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDoubleClickTime = 1; // Consider double click if it happens within 1 second
        io.FontGlobalScale = this->fontSize;
        io.IniFilename = NULL; // Deactivate ini file
        ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF( // Load JetBrainsMono
            jetbrains_mono_compressed_data,
            jetbrains_mono_compressed_size,
            constants::sizes::FONT
        );
        if (font == nullptr) {
            io.Fonts->AddFontDefault();
        }
        ImGui::StyleColorsDark(); // Set ImGUI dark theme
        ImGui_ImplGlfw_InitForOpenGL(this->app.windowManager->getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    Gui::~Gui() {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Gui::update() {
        // Start ImGui Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        this->drawMenuBar();

        switch (this->app.stateManager.getCurrentState()) {

            case AppState::WELCOME:
                this->drawWelcomeMessage();
                break;

            case AppState::GEOMETRY_CHOICE:
                this->drawGeometryFileDialog();
                break;

            case AppState::GEOMETRY_LOAD:
            case AppState::SHOW_GEOMETRY_LOAD:
                this->drawLoadingGeometry();
                break;

            case AppState::WAIT_GEOM_ABORT:
                this->drawGeometryError();
                break;

            default:
                this->drawInspector();
                this->drawRenderOptions();
                break;
        }
    }

    void Gui::render() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Gui::changeFontSize(float factor) {
        if (this->fontSize * factor < constants::limits::GUI_FONT_SIZE_MIN) return;
        this->fontSize *= factor;
        ImGuiIO& io = ImGui::GetIO();
        io.FontGlobalScale = this->fontSize;
    }

    bool Gui::isPointerOverGui() {
        return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    }

    void Gui::drawMenuBar() {
        bool interactionState = isInteractionState(this->app.stateManager.getCurrentState());
        if (ImGui::BeginMainMenuBar()) {
            this->menuBarHeight = ImGui::GetWindowSize().y;
            if (ImGui::BeginMenu("File")) {
                if (!interactionState) ImGui::BeginDisabled();
                if (ImGui::MenuItem("Save Image", "Ctrl + P")) {
                    this->app.stateManager.toggleImageExport();
                }
                if (!interactionState) ImGui::EndDisabled();
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "Alt + F4")) {
                    this->app.stateManager.close();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                if (!interactionState) ImGui::BeginDisabled();
                bool ortographic = this->app.scene->viewport->isOrthographic();
                if (ImGui::MenuItem("Ortographic projection", "P", ortographic)) {
                    this->app.scene->viewport->toggleProjectionType();
                }
                if (ImGui::BeginMenu("Standard views")) {
                    if (ImGui::MenuItem("Top (XZ)", "7"))   this->app.scene->viewport->setDirection(Camera::Directions::ALIGN_Y);
                    if (ImGui::MenuItem("Front (XY)", "1")) this->app.scene->viewport->setDirection(Camera::Directions::ALIGN_Z);
                    if (ImGui::MenuItem("Right (YZ)", "3")) this->app.scene->viewport->setDirection(Camera::Directions::ALIGN_X);
                    ImGui::Separator();
                    if (ImGui::MenuItem("Bottom (-XZ)", "Ctrl + 7"))  this->app.scene->viewport->setDirection(Camera::Directions::ALIGN_Y_NEG);
                    if (ImGui::MenuItem("Back (-XY)", "Ctrl + 1"))    this->app.scene->viewport->setDirection(Camera::Directions::ALIGN_Z_NEG);
                    if (ImGui::MenuItem("Left (-YZ)", "Ctrl + 3"))    this->app.scene->viewport->setDirection(Camera::Directions::ALIGN_X_NEG);
                    ImGui::Separator();
                    if (ImGui::MenuItem("Isometric", "0"))  this->app.scene->viewport->setDirection(Camera::Directions::ISOMETRIC);
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                bool inspector = this->app.settings.isSceneInspectorActive();
                if (ImGui::MenuItem("Scene inspector", "S", inspector)) {
                    this->app.settings.toggleSceneInspector();
                }
                bool renderOptions = this->app.settings.isRenderOptionsActive();
                if (ImGui::MenuItem("Render options", "G", renderOptions)) {
                    this->app.settings.toggleRenderOptions();
                }
                if (!interactionState) ImGui::EndDisabled();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Display")) {
                if (!interactionState) ImGui::BeginDisabled();
                bool pivot = this->app.settings.isCameraPivotActive();
                if (ImGui::MenuItem("Show Pivot", "X", pivot)) {
                    this->app.settings.toggleCameraPivot();
                }
                bool axis = this->app.settings.isAxisWidgetActive();
                if (ImGui::MenuItem("Show Axis", "A", axis)) {
                    this->app.settings.toggleAxisWidget();
                }
                if (!interactionState) ImGui::EndDisabled();
                ImGui::Separator();
                if (ImGui::MenuItem("Font +", "Ctrl + +")) {
                    this->changeFontSize(constants::factors::GUI_FONT_RESIZE);
                }
                if (ImGui::MenuItem("Font -", "Ctrl + -")) {
                    this->changeFontSize(1 / constants::factors::GUI_FONT_RESIZE);
                }
                bool vsyncEnabled = this->app.windowManager->isVsyncActive();
                if (ImGui::MenuItem("V-Sync", "V", vsyncEnabled)) {
                    this->app.windowManager->toggleVsync();
                }
                bool fullScreen = this->app.windowManager->isFullScreen();
                if (ImGui::MenuItem("Full Screen", "F11", fullScreen)) {
                    this->app.windowManager->toggleFullScreen();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Camera")) {
                if (!interactionState) ImGui::BeginDisabled();
                if (ImGui::MenuItem("Zoom In", "Scroll Up")) {
                    this->app.scene->viewport->zoom(constants::factors::ZOOM);
                }
                if (ImGui::MenuItem("Zoom Out", "Scroll Down")) {
                    this->app.scene->viewport->zoom(-constants::factors::ZOOM);
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Rotate Up", "Up")) {
                    this->app.scene->viewport->rotateByAngles(0, constants::factors::ROTATION_SPEED);
                }
                if (ImGui::MenuItem("Rotate Down", "Down")) {
                    this->app.scene->viewport->rotateByAngles(0, -constants::factors::ROTATION_SPEED);
                }
                if (ImGui::MenuItem("Rotate Right", "Right")) {
                    this->app.scene->viewport->rotateByAngles(constants::factors::ROTATION_SPEED, 0);
                }
                if (ImGui::MenuItem("Rotate Left", "Left")) {
                    this->app.scene->viewport->rotateByAngles(-constants::factors::ROTATION_SPEED, 0);
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Move Forward", "Shift + Scroll Up")) {
                    this->app.scene->viewport->movePerpendicular(0, constants::factors::PAN);
                }
                if (ImGui::MenuItem("Move Backward", "Shift + Scroll Down")) {
                    this->app.scene->viewport->movePerpendicular(0, -constants::factors::PAN);
                }
                if (ImGui::MenuItem("Move Up", "Shift + Up")) {
                    this->app.scene->viewport->moveParallel(0, constants::factors::PAN);
                }
                if (ImGui::MenuItem("Move Down", "Shift + Down")) {
                    this->app.scene->viewport->moveParallel(0, -constants::factors::PAN);
                }
                if (ImGui::MenuItem("Move Right", "Shift + Right")) {
                    this->app.scene->viewport->moveParallel(constants::factors::PAN, 0);
                }
                if (ImGui::MenuItem("Move Left", "Shift + Left")) {
                    this->app.scene->viewport->moveParallel(-constants::factors::PAN, 0);
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Reset Interaction", "ESC")) {
                    this->app.stateManager.resetInteraction();
                }
                if (ImGui::MenuItem("Reset Position", "R")) {
                    this->app.scene->viewport->reset();
                }
                if (!interactionState) ImGui::EndDisabled();
                ImGui::EndMenu();
            }

            // Move FPS label at the end of the window
            float fps = ImGui::GetIO().Framerate;
            char label[50];
            sprintf(label, "%s - %05.1f FPS", appStateToString(this->app.stateManager.getCurrentState()), fps);
            float textWidth = ImGui::CalcTextSize(label).x;
            float padding = ImGui::GetStyle().ItemSpacing.x;
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - textWidth - padding);
            ImGui::TextDisabled("%s", label);

            ImGui::EndMainMenuBar();
        }
    }

    void Gui::drawInspector() {
        bool open = this->app.settings.isSceneInspectorActive();

        if (open) {
            ImGui::SetNextWindowSizeConstraints(
                ImVec2(0.0f, 0.0f), // No min size
                ImVec2(
                    this->app.windowManager->getCurrentResolution().x - constants::sizes::PADDING * 2,
                    this->app.windowManager->getCurrentResolution().y - this->menuBarHeight - constants::sizes::PADDING * 2
                )
            );

            ImGui::SetNextWindowPos(ImVec2(constants::sizes::PADDING, this->menuBarHeight + constants::sizes::PADDING), ImGuiCond_Always);

            ImGui::SetNextWindowSize(ImVec2(400, this->app.windowManager->getCurrentResolution().y - this->menuBarHeight - constants::sizes::PADDING * 2), ImGuiCond_Once);

            ImGui::Begin("SCENE", &open, ImGuiWindowFlags_NoMove);

            this->drawObjectTree("SND", this->app.scene->detector.get());

            ImGui::End();
        }

        if (open != this->app.settings.isSceneInspectorActive()) this->app.settings.toggleSceneInspector();
    }

    void Gui::drawRenderOptions() {
        bool open = this->app.settings.isRenderOptionsActive();

        if (open) {
            ImGui::SetNextWindowSizeConstraints(
                ImVec2(0.0f, 0.0f), // No min size
                ImVec2(
                    this->app.windowManager->getCurrentResolution().x - constants::sizes::PADDING * 2,
                    this->app.windowManager->getCurrentResolution().y - this->menuBarHeight - constants::sizes::PADDING * 2
                )
            );

            ImGui::SetNextWindowPos(ImVec2(
                this->app.windowManager->getCurrentResolution().x - constants::sizes::PADDING,
                this->menuBarHeight + constants::sizes::PADDING),
                ImGuiCond_Always,
                ImVec2(1.0f, 0.0f) // Ser right pivot
            );

            ImGui::SetNextWindowSize(ImVec2(450, 0), ImGuiCond_Once);

            ImGui::Begin("RENDER OPTIONS", &open, ImGuiWindowFlags_NoMove);

            ImGui::Text("Projection Mode:");
            int projMode = this->app.scene->viewport->isOrthographic() ? 1 : 0;

            if (ImGui::RadioButton("Perspective", &projMode, 0)) {
                if (this->app.scene->viewport->isOrthographic()) {
                    this->app.scene->viewport->toggleProjectionType();
                }
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Orthographic", &projMode, 1)) {
                if (!this->app.scene->viewport->isOrthographic()) {
                    this->app.scene->viewport->toggleProjectionType();
                }
            }

            ImGui::NewLine();
            ImGui::Separator();
            ImGui::NewLine();

            bool transparency = this->app.settings.isTransparencyEnabled();
            if (ImGui::Checkbox("Transparency", &transparency)) {
                this->app.settings.toggleTransparency();
            }
            if (transparency) {
                ImGui::NewLine();
                float edgeAlphaValue = this->app.settings.getEdgeAlphaValue();
                if (ImGui::SliderFloat(" Edge Alpha", &edgeAlphaValue, constants::limits::ALPHA_VALUE_MIN, constants::limits::ALPHA_VALUE_MAX)) {
                    this->app.settings.setEdgeAlphaValue(edgeAlphaValue);
                }
                float faceAlphaValue = this->app.settings.getFaceAlphaValue();
                if (ImGui::SliderFloat(" Face Alpha", &faceAlphaValue, constants::limits::ALPHA_VALUE_MIN, constants::limits::ALPHA_VALUE_MAX)) {
                    this->app.settings.setFaceAlphaValue(faceAlphaValue);
                }
                float edgeThickness = this->app.settings.getEdgeThickness();
                if (ImGui::SliderFloat(" Edge Weight", &edgeThickness, constants::limits::EDGE_THICKNESS_MIN, constants::limits::EDGE_THICKNESS_MAX)) {
                    this->app.settings.setEdgeThickness(edgeThickness);
                }
                ImGui::NewLine();
                if (ImGui::Button("Reset")) {
                    this->app.settings.setEdgeAlphaValue(constants::defaults::EDGE_ALPHA_VALUE);
                    this->app.settings.setFaceAlphaValue(constants::defaults::FACE_ALPHA_VALUE);
                    this->app.settings.setEdgeThickness(constants::defaults::EDGE_THICKNESS);
                }
                ImGui::NewLine();
            }
            ImGui::End();
        }

        if (open != this->app.settings.isRenderOptionsActive()) this->app.settings.toggleRenderOptions();
    }

    void Gui::drawObjectTree(const std::string& label, Object* obj) {
        if (!obj || !obj->rootNode) return;

        ImGui::PushID(obj); // Create unique ID to identify the object into the gui

        ImGui::Checkbox("##objActive", &obj->active);
        ImGui::SameLine();

        if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

            ImGui::BeginChild((label + "Tree").c_str(), ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY, ImGuiWindowFlags_HorizontalScrollbar);

            if (ImGui::Button("Show All")) {
                obj->setGlobalActive(true);
            }
            ImGui::SameLine();
            if (ImGui::Button("Hide All")) {
                obj->setGlobalActive(false);
            }

            this->drawNodeTree(obj->rootNode.get()); // Start showing children recursively

            ImGui::EndChild();
        }

        ImGui::PopID(); // End using this object ID
    }

    void Gui::drawNodeTree(Node* node) {
        if (!node) return;

        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

        if (node->childrenNode.empty() && node->meshes.empty()) {
            nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }

        ImGui::PushID(node); // Create unique ID to identify the node into the gui

        ImGui::Checkbox("##nodeActive", &node->active);
        ImGui::SameLine();

        bool isOpened = ImGui::TreeNodeEx((void*)node, nodeFlags, "[Node] - %s", node->name.c_str()); // Create the tree for this node

        if (isOpened) {
            for (auto& mesh : node->meshes) {
                ImGui::PushID(mesh.get()); // Create unique ID to identify the mesh into the gui

                ImGuiTreeNodeFlags meshFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

                ImGui::Checkbox("##meshActive", &mesh->active);
                ImGui::SameLine();

                ImGui::TreeNodeEx((void*)mesh.get(), meshFlags, "[Mesh] - %s", mesh->name.c_str());

                ImGui::PopID();
            }

            for (auto& child : node->childrenNode) {
                drawNodeTree(child.get());
            }

            // If the node isn't a leaf, close the TreeNodeEx
            if (!(nodeFlags & ImGuiTreeNodeFlags_Leaf)) {
                ImGui::TreePop();
            }
        }

        ImGui::PopID();
    }

    void Gui::drawGeometryFileDialog() {

        ImGui::SetNextWindowPos(ImVec2(constants::sizes::PADDING, this->menuBarHeight + constants::sizes::PADDING), ImGuiCond_Always);

        ImGui::SetNextWindowSize(ImVec2(
            this->app.windowManager->getCurrentResolution().x - constants::sizes::PADDING * 2,
            this->app.windowManager->getCurrentResolution().y - this->menuBarHeight - constants::sizes::PADDING * 2),
            ImGuiCond_Once
        );

        if (ImGuiFileDialog::Instance()->Display(
            "ChooseGeometryFile",
            ImGuiWindowFlags_NoCollapse,
            ImVec2(0.0f, 0.0f),
            ImVec2(
                this->app.windowManager->getCurrentResolution().x - constants::sizes::PADDING * 2,
                this->app.windowManager->getCurrentResolution().y - this->menuBarHeight - constants::sizes::PADDING * 2
            )
        )) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

                this->app.stateManager.geometryFileSelected(filePathName);
            } else {
                this->app.stateManager.restart();
            }

            ImGuiFileDialog::Instance()->Close();
        }
    }

    void Gui::drawWelcomeMessage() {

        ImGui::SetNextWindowPos(ImVec2(constants::sizes::PADDING, this->menuBarHeight + constants::sizes::PADDING), ImGuiCond_Always);

        ImGui::SetNextWindowSize(ImVec2(
            this->app.windowManager->getCurrentResolution().x - constants::sizes::PADDING * 2,
            this->app.windowManager->getCurrentResolution().y - this->menuBarHeight - constants::sizes::PADDING * 2),
            ImGuiCond_Always
        );

        ImGui::Begin("WELCOME", NULL,
            ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
        );
        ImGui::TextWrapped("Open a geometry asset to initialize the 3D viewport.");
        ImGui::TextWrapped("Click the button below to browse your local files.");

        ImGui::NewLine();
        if (ImGui::Button("Browse")) {
            this->app.stateManager.openGeometryDialog();
            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseGeometryFile", "CHOOSE GEOMETRY FILE", ".gltf", config);
        }
        ImGui::NewLine();
        if (ImGui::Button("Quit")) {
            this->app.stateManager.close();
        }

        ImGui::End();
    }

    void Gui::drawLoadingGeometry() {

        ImGui::SetNextWindowPos(ImVec2(constants::sizes::PADDING, this->menuBarHeight + constants::sizes::PADDING), ImGuiCond_Always);

        ImGui::SetNextWindowSize(ImVec2(
            this->app.windowManager->getCurrentResolution().x - constants::sizes::PADDING * 2,
            this->app.windowManager->getCurrentResolution().y - this->menuBarHeight - constants::sizes::PADDING * 2),
            ImGuiCond_Always
        );

        ImGui::Begin("LOADING GEOMETRY", NULL,
            ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
        );
        ImGui::TextWrapped("Please Wait...");
        ImGui::NewLine();
        ImGui::TextWrapped("Loading geometry file:");
        ImGui::TextWrapped(this->app.stateManager.getDetectorPath().c_str());

        ImGui::End();
    }

    void Gui::drawGeometryError() {

        ImGui::SetNextWindowPos(ImVec2(constants::sizes::PADDING, this->menuBarHeight + constants::sizes::PADDING), ImGuiCond_Always);

        ImGui::SetNextWindowSize(ImVec2(
            this->app.windowManager->getCurrentResolution().x - constants::sizes::PADDING * 2,
            this->app.windowManager->getCurrentResolution().y - this->menuBarHeight - constants::sizes::PADDING * 2),
            ImGuiCond_Always
        );

        ImGui::Begin("ERROR LOADING GEOMETRY", NULL,
            ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
        );
        ImGui::TextWrapped("Error loading geometry file.");

        ImGui::NewLine();
        if (ImGui::Button("OK")) {
            this->app.stateManager.restart();
        }
        ImGui::NewLine();
        if (ImGui::Button("Quit")) {
            this->app.stateManager.close();
        }

        ImGui::End();
    }

}
