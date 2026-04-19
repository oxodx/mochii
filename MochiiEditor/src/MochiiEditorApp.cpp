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
};

Application* CreateApplication() { return new MochiiEditor(); }
}  // namespace Mochii
