/*
 Copyright (C) 2010 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Macros.h"
#include "ui/InputEvent.h"
#include "ui/InputState.h"

#include <vm/vec.h>

#include <memory>
#include <optional>
#include <string>

namespace tb::mdl
{
class PickResult;
}

namespace tb::render
{
class Camera;
class RenderBatch;
class RenderContext;
} // namespace tb::render

namespace tb::ui
{
class PickRequest;
class ToolController;
class ToolBox;
class ToolChain;

class ToolBoxConnector : public InputEventProcessor
{
private:
  ToolBox* m_toolBox = nullptr;
  std::unique_ptr<ToolChain> m_toolChain;

  InputState m_inputState;

  vm::vec2f m_lastMousePos = {0.0f, 0.0f};
  bool m_ignoreNextDrag = false;

  std::optional<vm::vec2f> m_lastGesturePanPos;

public:
  ToolBoxConnector();
  ~ToolBoxConnector() override;

public:
  const vm::ray3d& pickRay() const;
  const mdl::PickResult& pickResult() const;

  void updatePickResult();

protected:
  void setToolBox(ToolBox& toolBox);
  void addToolController(std::unique_ptr<ToolController> toolController);

public: // drag and drop
  bool dragEnter(float x, float y, const std::string& text);
  bool dragMove(float x, float y, const std::string& text);
  void dragLeave();
  bool dragDrop(float x, float y, const std::string& text);

public: // cancel
  bool cancel();

protected: // rendering
  void setRenderOptions(render::RenderContext& renderContext);
  void renderTools(
    render::RenderContext& renderContext, render::RenderBatch& renderBatch);

private:
  ModifierKeyState modifierKeys();
  bool setModifierKeys();

protected:
  bool clearModifierKeys();
  void updateModifierKeys();

private:
  void showPopupMenu();

public: // implement InputEventProcessor interface
  void processEvent(const KeyEvent& event) override;
  void processEvent(const MouseEvent& event) override;
  void processEvent(const ScrollEvent& event) override;
  void processEvent(const GestureEvent& event) override;
  void processEvent(const CancelEvent& event) override;

private:
  void processMouseButtonDown(const MouseEvent& event);
  void processMouseButtonUp(const MouseEvent& event);
  void processMouseClick(const MouseEvent& event);
  void processMouseDoubleClick(const MouseEvent& event);
  void processMouseMotion(const MouseEvent& event);
  void processDragStart(const MouseEvent& event);
  void processDrag(const MouseEvent& event);
  void processDragEnd(const MouseEvent& event);

  MouseButtonState mouseButton(const MouseEvent& event);
  void mouseMoved(float x, float y);

  void processGestureStart(const GestureEvent& event);
  void processGestureEnd(const GestureEvent& event);
  void processGesturePan(const GestureEvent& event);
  void processGestureZoom(const GestureEvent& event);
  void processGestureRotate(const GestureEvent& event);

public:
  bool cancelDrag();

private:
  virtual PickRequest pickRequest(float x, float y) const = 0;
  virtual mdl::PickResult pick(const vm::ray3d& pickRay) const = 0;
  virtual void doShowPopupMenu();

  deleteCopyAndMove(ToolBoxConnector);
};

} // namespace tb::ui
