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

#include "MapViewToolBox.h"

#include "mdl/EditorContext.h"
#include "ui/AssembleBrushTool.h"
#include "ui/ClipTool.h"
#include "ui/CreateEntityTool.h"
#include "ui/DrawShapeTool.h"
#include "ui/EdgeTool.h"
#include "ui/ExtrudeTool.h"
#include "ui/FaceTool.h"
#include "ui/MapDocument.h"
#include "ui/MoveObjectsTool.h"
#include "ui/RotateObjectsTool.h"
#include "ui/ScaleObjectsTool.h"
#include "ui/ShearObjectsTool.h"
#include "ui/VertexTool.h"

namespace tb::ui
{

MapViewToolBox::MapViewToolBox(
  std::weak_ptr<MapDocument> document, QStackedLayout* bookCtrl)
  : m_document{std::move(document)}
{
  createTools(bookCtrl);
  connectObservers();
}

MapViewToolBox::~MapViewToolBox() = default;

ClipTool& MapViewToolBox::clipTool()
{
  return *m_clipTool;
}

AssembleBrushTool& MapViewToolBox::assembleBrushTool()
{
  return *m_assembleBrushTool;
}

CreateEntityTool& MapViewToolBox::createEntityTool()
{
  return *m_createEntityTool;
}

DrawShapeTool& MapViewToolBox::drawShapeTool()
{
  return *m_drawShapeTool;
}

MoveObjectsTool& MapViewToolBox::moveObjectsTool()
{
  return *m_moveObjectsTool;
}

ExtrudeTool& MapViewToolBox::extrudeTool()
{
  return *m_extrudeTool;
}

RotateObjectsTool& MapViewToolBox::rotateObjectsTool()
{
  return *m_rotateObjectsTool;
}

ScaleObjectsTool& MapViewToolBox::scaleObjectsTool()
{
  return *m_scaleObjectsTool;
}

ShearObjectsTool& MapViewToolBox::shearObjectsTool()
{
  return *m_shearObjectsTool;
}

VertexTool& MapViewToolBox::vertexTool()
{
  return *m_vertexTool;
}

EdgeTool& MapViewToolBox::edgeTool()
{
  return *m_edgeTool;
}

FaceTool& MapViewToolBox::faceTool()
{
  return *m_faceTool;
}

void MapViewToolBox::toggleAssembleBrushTool()
{
  toggleTool(assembleBrushTool());
}

bool MapViewToolBox::assembleBrushToolActive() const
{
  return m_assembleBrushTool->active();
}

void MapViewToolBox::performAssembleBrush()
{
  m_assembleBrushTool->createBrushes();
}

void MapViewToolBox::toggleClipTool()
{
  toggleTool(clipTool());
}

bool MapViewToolBox::clipToolActive() const
{
  return m_clipTool->active();
}

void MapViewToolBox::toggleClipSide()
{
  assert(clipToolActive());
  m_clipTool->toggleSide();
}

void MapViewToolBox::performClip()
{
  assert(clipToolActive());
  m_clipTool->performClip();
}

void MapViewToolBox::removeLastClipPoint()
{
  assert(clipToolActive());
  m_clipTool->removeLastPoint();
}

void MapViewToolBox::toggleRotateObjectsTool()
{
  toggleTool(rotateObjectsTool());
}

bool MapViewToolBox::rotateObjectsToolActive() const
{
  return m_rotateObjectsTool->active();
}

double MapViewToolBox::rotateToolAngle() const
{
  assert(rotateObjectsToolActive());
  return m_rotateObjectsTool->angle();
}

vm::vec3d MapViewToolBox::rotateToolCenter() const
{
  assert(rotateObjectsToolActive());
  return m_rotateObjectsTool->rotationCenter();
}

void MapViewToolBox::moveRotationCenter(const vm::vec3d& delta)
{
  assert(rotateObjectsToolActive());
  const vm::vec3d center = m_rotateObjectsTool->rotationCenter();
  m_rotateObjectsTool->setRotationCenter(center + delta);
}

void MapViewToolBox::toggleScaleObjectsTool()
{
  toggleTool(scaleObjectsTool());
}

bool MapViewToolBox::scaleObjectsToolActive() const
{
  return m_scaleObjectsTool->active();
}

void MapViewToolBox::toggleShearObjectsTool()
{
  toggleTool(shearObjectsTool());
}

bool MapViewToolBox::shearObjectsToolActive() const
{
  return m_shearObjectsTool->active();
}

bool MapViewToolBox::anyVertexToolActive() const
{
  return vertexToolActive() || edgeToolActive() || faceToolActive();
}

void MapViewToolBox::toggleVertexTool()
{
  toggleTool(vertexTool());
}

bool MapViewToolBox::vertexToolActive() const
{
  return m_vertexTool->active();
}

void MapViewToolBox::toggleEdgeTool()
{
  toggleTool(edgeTool());
}

bool MapViewToolBox::edgeToolActive() const
{
  return m_edgeTool->active();
}

void MapViewToolBox::toggleFaceTool()
{
  toggleTool(faceTool());
}

bool MapViewToolBox::faceToolActive() const
{
  return m_faceTool->active();
}

bool MapViewToolBox::anyModalToolActive() const
{
  return rotateObjectsToolActive() || scaleObjectsToolActive() || shearObjectsToolActive()
         || anyVertexToolActive();
}

void MapViewToolBox::moveVertices(const vm::vec3d& delta)
{
  assert(anyVertexToolActive());
  if (vertexToolActive())
  {
    vertexTool().moveSelection(delta);
  }
  else if (edgeToolActive())
  {
    edgeTool().moveSelection(delta);
  }
  else if (faceToolActive())
  {
    faceTool().moveSelection(delta);
  }
}

void MapViewToolBox::createTools(QStackedLayout* bookCtrl)
{
  m_clipTool = std::make_unique<ClipTool>(m_document);
  m_assembleBrushTool = std::make_unique<AssembleBrushTool>(m_document);
  m_createEntityTool = std::make_unique<CreateEntityTool>(m_document);
  m_drawShapeTool = std::make_unique<DrawShapeTool>(m_document);
  m_moveObjectsTool = std::make_unique<MoveObjectsTool>(m_document);
  m_extrudeTool = std::make_unique<ExtrudeTool>(m_document);
  m_rotateObjectsTool = std::make_unique<RotateObjectsTool>(m_document);
  m_scaleObjectsTool = std::make_unique<ScaleObjectsTool>(m_document);
  m_shearObjectsTool = std::make_unique<ShearObjectsTool>(m_document);
  m_vertexTool = std::make_unique<VertexTool>(m_document);
  m_edgeTool = std::make_unique<EdgeTool>(m_document);
  m_faceTool = std::make_unique<FaceTool>(m_document);

  addExclusiveToolGroup(
    assembleBrushTool(),
    rotateObjectsTool(),
    scaleObjectsTool(),
    shearObjectsTool(),
    vertexTool(),
    edgeTool(),
    faceTool(),
    clipTool());

  suppressWhileActive(
    assembleBrushTool(), moveObjectsTool(), extrudeTool(), drawShapeTool());
  suppressWhileActive(
    rotateObjectsTool(), moveObjectsTool(), extrudeTool(), drawShapeTool());
  suppressWhileActive(
    scaleObjectsTool(), moveObjectsTool(), extrudeTool(), drawShapeTool());
  suppressWhileActive(
    shearObjectsTool(), moveObjectsTool(), extrudeTool(), drawShapeTool());
  suppressWhileActive(vertexTool(), moveObjectsTool(), extrudeTool(), drawShapeTool());
  suppressWhileActive(edgeTool(), moveObjectsTool(), extrudeTool(), drawShapeTool());
  suppressWhileActive(faceTool(), moveObjectsTool(), extrudeTool(), drawShapeTool());
  suppressWhileActive(clipTool(), moveObjectsTool(), extrudeTool(), drawShapeTool());

  registerTool(moveObjectsTool(), bookCtrl);
  registerTool(rotateObjectsTool(), bookCtrl);
  registerTool(scaleObjectsTool(), bookCtrl);
  registerTool(shearObjectsTool(), bookCtrl);
  registerTool(extrudeTool(), bookCtrl);
  registerTool(assembleBrushTool(), bookCtrl);
  registerTool(clipTool(), bookCtrl);
  registerTool(vertexTool(), bookCtrl);
  registerTool(edgeTool(), bookCtrl);
  registerTool(faceTool(), bookCtrl);
  registerTool(createEntityTool(), bookCtrl);
  registerTool(drawShapeTool(), bookCtrl);

  updateToolPage();
}

void MapViewToolBox::registerTool(Tool& tool, QStackedLayout* bookCtrl)
{
  tool.createPage(bookCtrl);
  addTool(tool);
}

void MapViewToolBox::connectObservers()
{
  m_notifierConnection +=
    toolActivatedNotifier.connect(this, &MapViewToolBox::toolActivated);
  m_notifierConnection +=
    toolDeactivatedNotifier.connect(this, &MapViewToolBox::toolDeactivated);

  auto document = kdl::mem_lock(m_document);
  m_notifierConnection += document->documentWasNewedNotifier.connect(
    this, &MapViewToolBox::documentWasNewedOrLoaded);
  m_notifierConnection += document->documentWasLoadedNotifier.connect(
    this, &MapViewToolBox::documentWasNewedOrLoaded);
  m_notifierConnection += document->selectionDidChangeNotifier.connect(
    this, &MapViewToolBox::selectionDidChange);
}

void MapViewToolBox::toolActivated(Tool&)
{
  updateEditorContext();
  updateToolPage();
}

void MapViewToolBox::toolDeactivated(Tool&)
{
  updateEditorContext();
  updateToolPage();
}

void MapViewToolBox::updateEditorContext()
{
  auto document = kdl::mem_lock(m_document);
  mdl::EditorContext& editorContext = document->editorContext();
  editorContext.setBlockSelection(assembleBrushToolActive());
}

void MapViewToolBox::documentWasNewedOrLoaded(MapDocument*)
{
  deactivateAllTools();
}

void MapViewToolBox::selectionDidChange(const Selection&)
{
  updateToolPage();
}

void MapViewToolBox::updateToolPage()
{
  if (rotateObjectsToolActive())
  {
    rotateObjectsTool().showPage();
  }
  else if (scaleObjectsToolActive())
  {
    scaleObjectsTool().showPage();
  }
  else if (shearObjectsToolActive())
  {
    shearObjectsTool().showPage();
  }
  else if (vertexToolActive())
  {
    vertexTool().showPage();
  }
  else if (edgeToolActive())
  {
    edgeTool().showPage();
  }
  else if (faceToolActive())
  {
    faceTool().showPage();
  }
  else if (clipToolActive())
  {
    clipTool().showPage();
  }
  else
  {
    drawShapeTool().showPage();
  }
}

} // namespace tb::ui
