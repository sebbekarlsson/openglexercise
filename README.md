## Requirements (Debian)
> Install these:

    apt-get install libglfw3-dev

    pip install --upgrade git+https://github.com/dav1dde/glad.git#egg=glad

    glad --generator=c --extensions=GL_EXT_framebuffer_multisample,GL_EXT_texture_filter_anisotropic --out-path=GL

## Notes
> Some notes:
* *GLAD* I am using *GLAD* to link OpenGL
* *GLFW3* I am using *GLFW* to create windows with OpenGL context

## Next up
> Next up is `EBO` (Element Buffer Objects)
