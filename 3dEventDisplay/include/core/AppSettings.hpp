#pragma once

#include "core/Constants.hpp"

namespace snd3D {
    class AppSettings {
        public:
            bool isCameraPivotActive();
            void toggleCameraPivot();
            bool isAxisWidgetActive();
            void toggleAxisWidget();
            bool isSceneInspectorActive();
            void toggleSceneInspector();
            bool isRenderOptionsActive();
            void toggleRenderOptions();
            bool isTransparencyEnabled();
            bool isTransparencyChanged();
            void toggleTransparency();
            float getEdgeAlphaValue();
            void setEdgeAlphaValue(const float alpha);
            float getFaceAlphaValue();
            void setFaceAlphaValue(const float alpha);
            float getEdgeThickness();
            void setEdgeThickness(const float thickness);

        private:
            bool cameraPivotActive = constants::defaults::SHOW_PIVOT;
            bool axisWidgetActive = constants::defaults::SHOW_AXIS;
            bool sceneInspectorActive = true;
            bool renderOptionsActive = true;
            bool transparency = constants::defaults::TRANSPARENCY, transparencyChanged = true;
            float edgeAlphaValue = constants::defaults::EDGE_ALPHA_VALUE;
            float faceAlphaValue = constants::defaults::FACE_ALPHA_VALUE;
            float edgeThickness = constants::defaults::EDGE_THICKNESS;
    };
}
