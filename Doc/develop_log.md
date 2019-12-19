### 开发日志

#### 20191219
* Error: no override found for 'vtkRenderer'
* Error: no override found for 'vtkContextDevice2D'

```
#include <vtkAutoInit.h> 
VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkRenderingContextOpenGL)
```
---






