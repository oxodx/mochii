#include <Mochii.h>
#include <Mochii/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Mochii {
class MochiiEditor : public Application {
 public:
  MochiiEditor() : Application("Mochii Editor") {
    PushLayer(new EditorLayer());
  }

  ~MochiiEditor() {}

  void OnMenuNewScene() override;
  void OnMenuOpenScene() override;
  void OnMenuSaveSceneAs() override;

 private:
  EditorLayer* GetEditorLayer() {
    for (auto* layer : GetLayerStack()) {
      if (auto* el = dynamic_cast<EditorLayer*>(layer)) return el;
    }
    return nullptr;
  }
};

Application* CreateApplication() { return new MochiiEditor(); }

void MochiiEditor::OnMenuNewScene() {
  if (auto* el = GetEditorLayer()) el->NewScene();
}

void MochiiEditor::OnMenuOpenScene() {
  if (auto* el = GetEditorLayer()) el->OpenScene();
}

void MochiiEditor::OnMenuSaveSceneAs() {
  if (auto* el = GetEditorLayer()) el->SaveSceneAs();
}
}  // namespace Mochii
