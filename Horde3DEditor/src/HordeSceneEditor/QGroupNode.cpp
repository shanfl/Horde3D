// ****************************************************************************************
//
// Horde3D Scene Editor 
// --------------------------------------
// Copyright (C) 2007 Volker Wiendl
// 
// This file is part of the Horde3D Scene Editor.
//
// The Horde3D Scene Editor is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 3 of the License 
//
// The Horde3D Scene Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ****************************************************************************************

#include "QGroupNode.h"

#include <QXmlTree/QXmlNodePropertyCommand.h>
#include "HordeSceneEditor.h"
#include "AttachmentPlugIn.h"
#include "SceneTreeModel.h"
#include "PlugInManager.h"

#include <float.h>
#include <QtCore/QTextStream>

#include "SceneFile.h"
#include <Horde3D.h>

QSceneNode* QGroupNode::loadNode(const QDomElement& xmlNode, int row, SceneTreeModel* model, QSceneNode* parent)
{
	return new QGroupNode(xmlNode, row, model, parent);
}

QGroupNode::QGroupNode(const QDomElement& xmlNode, int row, SceneTreeModel* model, QSceneNode* parentNode) : 
	QSceneNode(xmlNode, row, model, parentNode)
{
	setObjectName("Group");
	addRepresentation();
}

QGroupNode::~QGroupNode()
{	
}

void QGroupNode::addRepresentation()
{
	QSceneNode* parentNode = static_cast<QSceneNode*>(parent());
	unsigned int rootID = parentNode ? parentNode->hordeId() : H3DRootNode;

	m_hordeID = h3dAddGroupNode(rootID, qPrintable(m_xmlNode.attribute("name", "ATTENTION No Node Name")));

	float x, y, z, rx, ry, rz, sx, sy, sz;
	getTransformation(x,y,z,rx,ry,rz,sx,sy,sz);
	h3dSetNodeTransform(m_hordeID, x, y, z, rx, ry, rz, sx, sy, sz);

	// Attachment
	QDomElement attachment = m_xmlNode.firstChildElement("Attachment");	
	SceneTreeModel* model = static_cast<SceneTreeModel*>(m_model);
	AttachmentPlugIn* plugIn = model->nodeFactory()->attachmentPlugIn();
	if (!attachment.isNull() &&  plugIn != 0)
		plugIn->initNodeAttachment(this);
}

