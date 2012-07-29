/*
 Copyright (C) 2010-2012 Kristian Duske
 
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
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_Selection_h
#define TrenchBroom_Selection_h

#include <vector>
#include "Model/Assets/Texture.h"
#include "Model/Map/Brush.h"
#include "Model/Map/Face.h"
#include "Model/Map/Entity.h"
#include "Utilities/Event.h"
#include "Utilities/VecMath.h"

namespace TrenchBroom {
    namespace Model {
        typedef enum {
            TB_SM_NONE,
            TB_SM_FACES,
            TB_SM_BRUSHES,
            TB_SM_ENTITIES,
            TB_SM_BRUSHES_ENTITIES
        } ESelectionMode;
        
        class Texture;
        class Entity;
        class Brush;
        class Face;
        
        class SelectionState {
        public:
            EntityList entities;
            BrushList brushes;
            BrushList partialBrushes;
            FaceList faces;
            std::vector<Assets::Texture*> mruTextures;
            ESelectionMode mode;
            
            SelectionState() : mode(TB_SM_NONE) {}
            SelectionState(const SelectionState& state) : entities(state.entities), brushes(state.brushes), partialBrushes(state.partialBrushes), faces(state.faces), mruTextures(state.mruTextures), mode(state.mode) {}

            void clear() {
                entities.clear();
                brushes.clear();
                partialBrushes.clear();
                faces.clear();
                mruTextures.clear();
                mode = TB_SM_NONE;
            }
        };
        
        class SelectionEventData {
        public:
            EntityList entities;
            BrushList brushes;
            FaceList faces;
            SelectionEventData() {};
            SelectionEventData(const EntityList& entities) : entities(entities) {}
            SelectionEventData(const EntityList& entities, const BrushList& brushes) : entities(entities), brushes(brushes) {}
            SelectionEventData(const BrushList& brushes) : brushes(brushes) {}
            SelectionEventData(const FaceList& faces) : faces(faces) {}
            SelectionEventData(Entity& entity) { entities.push_back(&entity); }
            SelectionEventData(Brush& brush) { brushes.push_back(&brush); }
            SelectionEventData(Face& face) { faces.push_back(&face); }
        };
        
        class Selection {
        private:
            mutable std::vector<SelectionState> m_state;
            
            inline SelectionState& current() const {
                return m_state.back();
            }
        protected:
            void doAddEntities(const EntityList& entities);
            void doAddBrushes(const BrushList& brushes);
            void doAddFaces(const FaceList& faces);
            EntityList doRemoveEntities(const EntityList& entities);
            BrushList doRemoveBrushes(const BrushList& brushes);
            FaceList doRemoveFaces(const FaceList& faces);
        public:
            typedef Event<const SelectionEventData&> SelectionEvent;
            SelectionEvent selectionAdded;
            SelectionEvent selectionRemoved;
            
            inline Selection() {
                m_state.resize(1);
            }
            
            void push();
            void pop();
            
            inline ESelectionMode mode() const {
                return current().mode;
            }
            
            inline bool empty() const {
                return current().entities.empty() && current().brushes.empty() && current().faces.empty();
            }
            
            inline const std::vector<Assets::Texture*>& mruTextures() const {
                return current().mruTextures;
            }
            
            inline Assets::Texture* texture() const {
                if (current().mruTextures.empty())
                    return NULL;
                return current().mruTextures.back();
            }
            
            inline const FaceList& faces() const {
                return current().faces;
            }
            
            const FaceList brushFaces() const;
            const FaceList allFaces() const;
            
            const BrushList& brushes() const {
                return current().brushes;
            }
            
            const BrushList& partialBrushes() const {
                return current().partialBrushes;
            }
            
            const EntityList& entities() const {
                return current().entities;
            }
            
            const Entity* brushSelectionEntity() const;
            Vec3f center() const;
            BBox bounds() const;
            
            void addTexture(Assets::Texture& texture);
            void addFace(Face& face);
            void addFaces(const FaceList& faces);
            void addBrush(Brush& brush);
            void addBrushes(const BrushList& brushes);
            void addEntity(Entity& entity);
            void addEntities(const EntityList& entities);
            
            void replaceSelection(const EntityList& entities, const BrushList& brushes);
            void replaceSelection(const EntityList& entities);
            void replaceSelection(const BrushList& brushes);
            void replaceSelection(Entity& entity);
            void replaceSelection(Brush& brush);
            void replaceSelection(const FaceList& faces);
            void replaceSelection(Face& face);
            
            void removeFace(Face& face);
            void removeFaces(const FaceList& faces);
            void removeBrush(Brush& brush);
            void removeBrushes(const BrushList& brushes);
            void removeEntity(Entity& entity);
            void removeEntities(const EntityList& entities);
            void removeAll();
        };
    }
}

#endif
