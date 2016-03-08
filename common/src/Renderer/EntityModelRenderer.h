/*
 Copyright (C) 2010-2014 Kristian Duske
 
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

#ifndef __TrenchBroom__EntityModelRenderer__
#define __TrenchBroom__EntityModelRenderer__

#include "Color.h"
#include "Assets/ModelDefinition.h"
#include "Model/ModelTypes.h"
#include "Renderer/Renderable.h"

#include <map>
#include <set>

namespace TrenchBroom {
    namespace Assets {
        class EntityModel;
        class EntityModelManager;
    }
    
    namespace Model {
        class EditorContext;
        class Entity;
    }
    
    namespace Renderer {
        class TexturedTriangleMeshRenderer;
        class RenderBatch;
        class RenderContext;
        
        class EntityModelRenderer : public Renderable {
        private:
            typedef std::map<Model::Entity*, TexturedTriangleMeshRenderer*> EntityMap;
            
            Assets::EntityModelManager& m_entityModelManager;
            const Model::EditorContext& m_editorContext;
            
            EntityMap m_entities;
            
            bool m_applyTinting;
            Color m_tintColor;
            
            bool m_showHiddenEntities;
        public:
            EntityModelRenderer(Assets::EntityModelManager& entityModelManager, const Model::EditorContext& editorContext);
            ~EntityModelRenderer();
            
            template <typename I>
            void setEntities(I cur, I end) {
                clear();
                addEntities(cur, end);
            }
            
            template <typename I>
            void addEntities(I cur, I end) {
                while (cur != end) {
                    addEntity(*cur);
                    ++cur;
                }
            }
            
            void addEntity(Model::Entity* entity);
            void clear();
            
            bool applyTinting() const;
            void setApplyTinting(const bool applyTinting);
            const Color& tintColor() const;
            void setTintColor(const Color& tintColor);
            
            bool showHiddenEntities() const;
            void setShowHiddenEntities(bool showHiddenEntities);
            
            void render(RenderBatch& renderBatch);
        private:
            void doPrepare(Vbo& vbo);
            
            class MeshFunc;
            void doRender(RenderContext& renderContext);
        };
    }
}

#endif /* defined(__TrenchBroom__EntityModelRenderer__) */