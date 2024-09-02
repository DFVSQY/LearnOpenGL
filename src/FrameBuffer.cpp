#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
    /*
     * glGenFramebuffers 是 OpenGL 中的一个函数，用于生成一个或多个新的 Framebuffer Object (FBO)。
     * FBO 是 OpenGL 提供的一种机制，允许程序将渲染目标从默认的屏幕切换到一个离屏缓冲区（off-screen buffer），从而支持更复杂的渲染技术，
     * 如阴影映射、后处理效果、反射等。

     * 函数原型：void glGenFramebuffers(GLsizei n, GLuint *framebuffers);
     * 参数详解
     *  n：GLsizei 类型，表示要生成的 FBO 的数量。n 必须是一个正整数。
     *  framebuffers：GLuint* 类型，指向一个数组的指针，数组的大小至少为 n。函数将在该数组中存储生成的 FBO 的句柄（即 ID）。
     *              这些句柄可以用于后续的绑定、配置和使用。

     * glGenFramebuffers 的作用是生成 n 个新的 FBO，并将它们的句柄（ID）存储到 framebuffers 数组中。
     * 每个 FBO 是一个用于离屏渲染的对象，可以附加颜色缓冲区、深度缓冲区和模板缓冲区等附件。

     * 工作原理
     *  创建唯一 ID：OpenGL 内部会生成 n 个唯一的 ID，这些 ID 是 FBO 的句柄，用于引用这些 FBO 对象。
     *  分配资源：这些 ID 还没有真正分配具体的资源或配置，只有在后续调用 glBindFramebuffer 并对其进行配置时，才会真正分配所需的资源。
     *  返回 ID：这些生成的 ID 被存储在传入的 framebuffers 数组中，程序可以使用这些 ID 来引用和操作对应的 FBO。

     * 注意事项
     *  生成 vs. 使用：glGenFramebuffers 仅生成 FBO 的 ID，而不配置其附件。
     *  要实际使用 FBO，需要调用 glBindFramebuffer 绑定该 FBO，然后使用 glFramebufferTexture2D 或 glFramebufferRenderbuffer 来附加纹理或渲染缓冲区。
     *  删除 FBO：在不再需要 FBO 时，可以使用 glDeleteFramebuffers 删除它们，以释放资源。
     *  FBO 的状态：一个 FBO 只有在附加了至少一个附件后，才可以被用于渲染。
     *  创建 FBO 后，通常需要检查它的状态，以确保它是"完整的"（complete），可以通过调用 glCheckFramebufferStatus 来检查。
    */
    glGenFramebuffers(1, &m_fbo);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_fbo);

    for (auto texture : m_textures)
    {
        glDeleteTextures(1, &texture);
    }

    for (auto renderBuffer : m_renderBuffers)
    {
        glDeleteRenderbuffers(1, &renderBuffer);
    }
}

void FrameBuffer::Bind() const
{
    /*
     * glBindFramebuffer 是 OpenGL 中的一个函数，用于绑定一个 Framebuffer Object (FBO) 到当前的 OpenGL 上下文，使其成为当前的渲染目标。
     * 绑定操作使得所有后续的渲染操作都会影响到这个 FBO 而不是默认的窗口帧缓冲区。

     * 函数原型：void glBindFramebuffer(GLenum target, GLuint framebuffer);
     * 参数详解
     *  target：指定要绑定的帧缓冲区的目标。通常使用以下三个选项之一：
     *      GL_FRAMEBUFFER：通用目标，适用于读取和绘制操作。
     *      GL_DRAW_FRAMEBUFFER：只用于绘制操作。
     *      GL_READ_FRAMEBUFFER：只用于读取操作。
     *      对于大多数情况下，使用 GL_FRAMEBUFFER 即可。
     *  framebuffer：要绑定的 FBO 的句柄（ID），由 glGenFramebuffers 函数生成。如果传入 0，则表示绑定到默认的帧缓冲区，即屏幕。

     * 函数的作用
     *  glBindFramebuffer 的作用是将指定的 FBO 绑定到当前的 OpenGL 上下文，使其成为当前的渲染目标。
     *  绑定 FBO 后，所有后续的渲染操作（如绘制三角形、清除缓冲区等）都会应用到这个 FBO 上，而不是默认的窗口帧缓冲区。

     * 工作原理
     *  切换渲染目标：当调用 glBindFramebuffer 时，OpenGL 将切换当前的渲染目标。后续的所有绘图操作（如 glDraw* 函数调用）都会影响到这个 FBO 的附加缓冲区（颜色缓冲区、深度缓冲区等）。
     *  影响 OpenGL 状态：绑定 FBO 也会影响 OpenGL 的其他状态，如视口大小（glViewport）以及裁剪矩形（scissor box），这些状态可能需要根据 FBO 的尺寸进行调整。
     *  0 表示恢复默认帧缓冲区：当 framebuffer 参数为 0 时，glBindFramebuffer 会将渲染目标恢复为默认的窗口帧缓冲区，这意味着后续的渲染操作会直接显示在屏幕上。

     * 注意事项
     *  FBO 的完整性检查：绑定 FBO 后，应该使用 glCheckFramebufferStatus 检查其是否完整（GL_FRAMEBUFFER_COMPLETE），确保它可以用于渲染。
     *  解绑 FBO：在完成离屏渲染后，通常需要将 FBO 解绑（通过 glBindFramebuffer(GL_FRAMEBUFFER, 0)），以恢复到默认的帧缓冲区。这也是一个好习惯，以确保不会意外地继续向 FBO 渲染。
     *  影响渲染状态：绑定 FBO 可能会改变视口和其他 OpenGL 状态，特别是在切换回默认帧缓冲区时，应注意重置这些状态。
    */
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBuffer::Unbind()
{
    /*
     * 恢复到默认的帧缓冲区
    */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
 * 添加Texture Object附件
*/
void FrameBuffer::AttachTexture(GLenum attachment, GLenum internalFormat, GLenum format, GLenum type, GLsizei width,
                                GLsizei height)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*
     * glFramebufferTexture2D 是 OpenGL 中用于将一个二维纹理（2D Texture）附加到 Framebuffer Object (FBO) 的特定附件点上的函数。
     * 这个操作使得该纹理可以作为 FBO 的渲染目标，从而使得渲染的结果可以存储在纹理中，而不是直接输出到屏幕。

     * 函数原型：void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
     * 参数详解
     *  target：指定要绑定的帧缓冲区的目标。对于 FBO 通常使用 GL_FRAMEBUFFER。
     *  attachment：指定纹理将要附加到 FBO 的哪个附件点上。常用的附件点包括：
     *      GL_COLOR_ATTACHMENT0：附加为颜色附件（通常用于存储渲染的颜色数据）。
     *      GL_DEPTH_ATTACHMENT：附加为深度附件（用于存储深度信息）。
     *      GL_STENCIL_ATTACHMENT：附加为模板附件（用于存储模板信息）。
     *      GL_DEPTH_STENCIL_ATTACHMENT：附加为深度和模板附件（用于同时存储深度和模板信息）。
     *      如果需要在同一个 FBO 中使用多个颜色附件，您可以使用 GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 等。
     *  textarget：指定纹理的目标类型，通常为 GL_TEXTURE_2D，表示二维纹理。它的值应与 texture 参数所引用的纹理的类型一致。
     *  texture：要附加到 FBO 的纹理对象的句柄（ID）。这个纹理对象必须是一个有效的、之前已经通过 glGenTextures 创建的二维纹理。
     *  level：指定要附加的纹理的 mipmap 级别，通常为 0，表示基础级别（即最高分辨率的 mipmap 级别）。
    */
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);

    m_textures.push_back(texture);
}

/*
 * 添加 RenderBuffer Object附件
*/
void FrameBuffer::AttachRenderBuffer(GLenum attachment, GLenum internalFormat, GLsizei width, GLsizei height)
{
    GLuint renderBuffer;

    /*
     * glGenRenderbuffers 是 OpenGL 中用于创建一个或多个渲染缓冲区对象（Renderbuffer Objects）的函数。
     * 渲染缓冲区对象是 OpenGL 提供的一种特殊的缓冲区类型，通常用于存储渲染的中间数据，如深度、模板（Stencil）或多重采样（Multisampling）信息。

     * 函数原型：void glGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
     *  参数详解
     *      n: 指定要生成的渲染缓冲区对象的数量。你可以生成一个或多个渲染缓冲区对象。
     *      renderbuffers: 指向一个 GLuint 数组，该数组的大小至少为 n。在函数调用后，这个数组中会存储生成的渲染缓冲区对象的句柄（即 ID）。

     * 函数的作用
     *  glGenRenderbuffers 的作用是生成一个或多个渲染缓冲区对象的 ID，这些 ID 可以在后续的 OpenGL 调用中使用，以管理和操作这些渲染缓冲区。

     * 渲染缓冲区对象的用途
     *  渲染缓冲区对象主要用于以下几种场景：
     *      深度缓冲（Depth Buffer）:
     *          用于存储深度信息，即每个像素的深度值，通常在深度测试过程中使用。
     *          例如，用于确保物体在场景中的正确遮挡关系。
     *      模板缓冲（Stencil Buffer）:
     *          用于存储模板信息，用于复杂的遮罩操作、镜像效果、阴影体积等。
     *          例如，利用模板缓冲区实现特定区域的渲染限制。
     *      多重采样缓冲（Multisample Buffer）:
     *          用于存储多重采样数据，以实现抗锯齿效果。渲染缓冲区可以保存多重采样的颜色或深度数据。
     *          例如，通过多重采样提高图像的抗锯齿质量。
    */
    glGenRenderbuffers(1, &renderBuffer);

    /*
     * glBindRenderbuffer 是 OpenGL 提供的函数，用于绑定一个渲染缓冲对象（Renderbuffer）。
     * 渲染缓冲对象通常用于创建用于离屏渲染的缓冲区，尤其是在涉及到多重采样或深度、模板缓冲时，Renderbuffer 是更常用的选择。

     * 函数原型：void glBindRenderbuffer(GLenum target, GLuint renderbuffer);
     * 参数解释
     *  target: 一般来说，这个参数的值总是 GL_RENDERBUFFER，因为 Renderbuffer 对象只能绑定到 GL_RENDERBUFFER 这个目标。
     *  renderbuffer: 这是要绑定的渲染缓冲对象的名称（ID）。如果传入 0，表示解除绑定当前的 Renderbuffer 对象。

     * 函数的作用
     * glBindRenderbuffer 函数的主要作用是将指定的渲染缓冲对象绑定到当前的 OpenGL 上下文，
     * 使得之后的所有与渲染缓冲对象相关的操作都会作用在这个绑定的对象上。通过绑定操作，OpenGL 可以跟踪并使用指定的渲染缓冲区对象。

     * 渲染缓冲对象的典型用途
     *  渲染缓冲对象主要用于两种情况：
     *      深度和模板缓冲：当你需要一个专门的深度缓冲区（depth buffer）或模板缓冲区（stencil buffer）时，渲染缓冲对象通常比纹理对象更合适。与纹理不同，渲染缓冲对象通常不会被采样，但它们性能优化，特别是用于多重采样（MSAA）时。
     *      多重采样缓冲区（MSAA）：多重采样抗锯齿的场景中，渲染缓冲对象常用于存储多重采样的数据。由于纹理对象不支持多重采样，而渲染缓冲对象则可以存储这些采样结果。

     * 注意事项
     *  glBindRenderbuffer 仅负责绑定渲染缓冲对象，它并不会直接创建或分配任何存储空间。存储空间的分配是通过 glRenderbufferStorage 函数完成的。
     *  Renderbuffer 对象无法像纹理一样被采样，这使得它更适合用于不需要读取数据的场景，如深度缓冲、模板缓冲或作为多重采样存储。
     *  一旦 Renderbuffer 被绑定，所有对渲染缓冲对象的操作都会作用于这个绑定的对象，直到再次绑定另一个 Renderbuffer。
    */
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);

    /*
     * glRenderbufferStorage 是 OpenGL 中用于为绑定的渲染缓冲对象（Renderbuffer）分配存储空间的函数。
     * 渲染缓冲对象通常用于离屏渲染操作，特别是在需要深度缓冲、模板缓冲或者多重采样（MSAA）时。glRenderbufferStorage 确定了渲染缓冲的格式和尺寸。

     * 函数原型：void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
     * 参数详解
     *  target:
     *      必须为 GL_RENDERBUFFER，指定目标是渲染缓冲对象。
     *
     *  internalformat:
     *      指定渲染缓冲对象的内部格式，这决定了存储的数据类型及其每个像素占用的位数。常见的内部格式包括：
     *      GL_DEPTH_COMPONENT: 深度缓冲（通常是24位或32位）。
     *      GL_STENCIL_INDEX: 模板缓冲（通常是8位）。
     *      GL_DEPTH24_STENCIL8: 同时包含深度和模板的缓冲区（24位深度 + 8位模板）。
     *      GL_RGBA8: 颜色缓冲区（8位每个颜色通道，RGBA）。
     *      GL_RGB565: 颜色缓冲区（5位红色、6位绿色和5位蓝色）。
     *      选择合适的内部格式非常重要，因为它会影响渲染效果和性能。
     *
     *  width:
     *      渲染缓冲区的宽度，以像素为单位。
     *
     *  height:
     *      渲染缓冲区的高度，以像素为单位。

     * 函数的作用
     *  glRenderbufferStorage 的作用是为当前绑定的渲染缓冲对象分配存储空间，并确定该缓冲区的格式及其尺寸。
     *  在调用 glRenderbufferStorage 之前，渲染缓冲对象只是一个空壳，它没有实际的存储空间。通过这个函数，OpenGL 分配了特定的显存资源，用于存储渲染操作生成的数据（如深度值、颜色值等）。

     * glRenderbufferStorage 是用于分配和设置渲染缓冲对象存储空间的关键函数。它决定了渲染缓冲区的格式、大小，以及其具体用途（如深度缓冲、模板缓冲等）。
     * 在离屏渲染、多重采样、深度模板测试等场景中，glRenderbufferStorage 发挥了至关重要的作用。
    */
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);

    /*
     * glFramebufferRenderbuffer 是 OpenGL 中的一个函数，用于将一个渲染缓冲对象（Renderbuffer）附加到帧缓冲对象（Framebuffer）上。
     * 这个操作通常用于离屏渲染场景，尤其是在需要使用深度缓冲、模板缓冲或多重采样时。

     * 函数原型：void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
     * 参数详解
     *  target:
     *      这个参数指定帧缓冲对象的目标，通常是 GL_FRAMEBUFFER，表示应用于当前绑定的帧缓冲对象。
     *
     *  attachment:
     *      这个参数指定将渲染缓冲对象附加到帧缓冲对象的哪个附件点。常用的值包括：
     *      GL_COLOR_ATTACHMENTi: 将渲染缓冲对象附加为第 i 个颜色附件，其中 i 是颜色附件的编号。
     *      GL_DEPTH_ATTACHMENT: 将渲染缓冲对象附加为深度附件，用于存储深度值。
     *      GL_STENCIL_ATTACHMENT: 将渲染缓冲对象附加为模板附件，用于存储模板值。
     *      GL_DEPTH_STENCIL_ATTACHMENT: 将渲染缓冲对象附加为同时包含深度和模板的附件。
     *
     *  renderbuffertarget:
     *      这个参数必须为 GL_RENDERBUFFER，指定附加的目标类型是渲染缓冲对象。
     *
     *  renderbuffer:
     *      这是渲染缓冲对象的 ID，即要附加的渲染缓冲对象。如果传入 0，则会解除与该附件点的绑定。

     * 函数的作用
     *  glFramebufferRenderbuffer 的作用是将一个渲染缓冲对象附加到当前绑定的帧缓冲对象的指定附件点。
     *  帧缓冲对象本质上是一个自定义的、可供渲染的缓冲集合，而渲染缓冲对象则是这些缓冲的一部分，用于特定的渲染目的（如深度、模板、多重采样等）。
     *  通过这个函数，OpenGL 可以在渲染到帧缓冲对象时，将生成的渲染数据（如深度值或模板值）存储到附加的渲染缓冲区中。
    */
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer);

    m_renderBuffers.push_back(renderBuffer);
}

bool FrameBuffer::IsComplete() const
{
    /*
     * glCheckFramebufferStatus 是 OpenGL 中用于检查帧缓冲对象（Framebuffer Object, FBO）是否完整的函数。
     * 帧缓冲对象的完整性对于确保渲染操作能正确进行至关重要，因此在配置完帧缓冲对象后，使用 glCheckFramebufferStatus 函数来验证其状态是非常重要的步骤。

     * 函数原型：GLenum glCheckFramebufferStatus(GLenum target);
     * 参数详解
     *  target:
     *      这个参数指定要检查的帧缓冲对象的目标。通常为 GL_FRAMEBUFFER，表示检查当前绑定的帧缓冲对象。

     * 返回值
     *  glCheckFramebufferStatus 返回一个 GLenum 类型的值，用于指示帧缓冲对象的状态。可能的返回值包括以下几种：
     *
     *  GL_FRAMEBUFFER_COMPLETE:
     *  表示帧缓冲对象是完整的，可以安全地用于渲染操作。这是你期望得到的结果。
     *
     *  GL_FRAMEBUFFER_UNDEFINED:
     *  在 OpenGL ES 或部分桌面 OpenGL 中，这个状态表示默认帧缓冲对象的 target 参数未定义。这通常出现在尝试在不支持的环境中操作帧缓冲对象时。
     *
     *  GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
     *  表示帧缓冲对象附加的某个或多个附件是不完整的或有问题的。比如，一个颜色附件的格式与其他附件不兼容，或者附加的纹理或渲染缓冲对象是未完成的或无效的。
     *
     *  GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
     *  表示帧缓冲对象没有附加任何图像数据对象。通常，这是因为没有附加任何颜色、深度或模板缓冲对象。
     *
     *  GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
     *  当当前绑定的帧缓冲对象的某个 GL_DRAW_BUFFER 指向的附件不存在或不可用时，会返回此状态。这通常发生在多个渲染目标（MRT）配置不当时。
     *
     *  GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
     *  当 GL_READ_BUFFER 指向的附件不存在或不可用时，会返回此状态。这通常发生在读取操作配置不当时。
     *
     *  GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
     *  如果多重采样相关的附件之间的样本数或存储格式不一致时，会返回此状态。这个错误通常发生在配置多重采样缓冲区时。
     *
     *  GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
     *  当帧缓冲对象的某些附件是层目标（如立方体贴图或 3D 纹理），但层的选择或配置不一致时，会返回此状态。
     *
     *  GL_FRAMEBUFFER_UNSUPPORTED:
     *  表示帧缓冲对象的配置组合（比如附件格式和内部格式）不被当前的 OpenGL 实现支持。这通常需要重新配置帧缓冲对象的格式或结构。
    */
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}
