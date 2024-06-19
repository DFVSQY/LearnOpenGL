#include <iostream>
#include "Shader.h"
#include "Mesh.h"

Mesh::Mesh() : vao(0), vbo(0), ebo(0), index_num(0)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Init(const GLfloat *vertices, const int vertexNum)
{
    if (vertices == nullptr || vertexNum <= 0)
    {
        std::cerr << "Mesh::Init() failed: vertices is null or empty." << std::endl;
        return false;
    }

    SetupVAO(vertices, vertexNum);
    if (vao == 0 || vbo == 0)
    {
        std::cerr << "Mesh::Init() failed: SetupVAO() returned 0." << std::endl;
        return false;
    }

    index_num = 0;

    return true;
}

bool Mesh::Init_Elements(const GLfloat *vertices, const GLuint *indices, const int vertexNum, const int indexNum)
{
    if (vertices == nullptr || vertexNum <= 0)
    {
        std::cerr << "Mesh::Init_Elements() failed: vertices is null or empty." << std::endl;
        return false;
    }

    if (indices == nullptr || indexNum <= 0)
    {
        std::cerr << "Mesh::Init_Elements() failed: indices is null or empty." << std::endl;
        return false;
    }

    SetupVAO_Elements(vertices, vertexNum, indices, indexNum);
    if (vao == 0 || vbo == 0 || ebo == 0)
    {
        std::cerr << "Mesh::Init_Elements() failed: SetupVAO_Elements() returned 0." << std::endl;
        return false;
    }

    index_num = vertexNum;

    return true;
}

void Mesh::Draw() const
{
    if (vao == 0)
        return;

    // draw mesh content
    glBindVertexArray(vao);

    if (index_num > 0)
    {
        /*
         * glDrawElements是OpenGL中用于根据索引数据绘制图元的函数。它使用存储在元素数组缓冲区（Element Array Buffer）中的索引来指定绘制哪些顶点。

         * 函数原型：void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
         *  mode：指定要绘制的图元类型。
         *  count：指定要渲染的元素（顶点）的数量。它表示索引数组中的索引数
         *  type：指定索引数据的类型。
         *  indices：指向索引数据的指针。它可以是以下三种情况之一：
         *      1. 如果当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的缓冲区对象是非零的，则indices参数被解释为指向缓冲区对象数据存储区起始位置的偏移量。
         *      2. 如果当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的缓冲区对象是零，则indices参数被解释为指向客户端内存中索引数组的指针。
         *      3. 如果使用OpenGL 4.5或更高版本，并且启用了直接状态访问（Direct State Access），则indices参数可以是指向包含索引数据的缓冲区对象的非零名称（ID）。

         * glDrawElements函数的工作原理如下：
         *  1. 根据mode参数指定的图元类型，确定如何解释索引数据和绘制图元。
         *  2. 根据count参数指定的元素数量，确定要绘制的顶点数。
         *  3. 根据type参数指定的索引数据类型，确定如何解释indices参数指向的索引数据。
         *  4. 根据当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的缓冲区对象，确定索引数据的来源（缓冲区对象或客户端内存）。
         *  5. 使用索引数据从绑定到GL_ARRAY_BUFFER目标的缓冲区对象或客户端内存中的顶点数组中获取顶点数据。
         *  6. 根据索引数据和图元类型，绘制指定的图元。
        */
        glDrawElements(GL_TRIANGLES, index_num, GL_UNSIGNED_INT, 0);
    }
    else
    {
        /*
        * glDrawArrays 是 OpenGL 中的一个函数，用于绘制图元（如点、线、三角形等）。
        * 它根据当前绑定的顶点数组对象 (VAO) 和顶点缓冲对象 (VBO) 中的数据，按照指定的图元类型和顶点数量进行绘制。

        * 有三个参数：mode, first, count：
        *  mode：指定要绘制的图元类型。GL_TRIANGLES：绘制独立的三角形。
        *  first：指定第一个顶点的索引。它表示从哪个位置开始读取顶点数据。
        *  count：指定顶点数量。

        * glDrawArrays 的工作流程：
        *  1. 绑定 VAO：确保当前绑定的 VAO 包含所需的顶点属性配置和 VBO 绑定。
        *  2. 读取顶点数据：从绑定的 VBO 中读取顶点数据，从索引 first 开始，读取 count 个顶点。
        *  3. 组装图元：根据 mode 参数指定的图元类型，将读取的顶点数据组装成相应的图元（如点、线、三角形等）。
        *  4. 绘制图元：发送组装好的图元到 GPU 进行渲染。
        */
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

void Mesh::SetupVAO(const GLfloat *vertices, int vertexNum)
{
    if (vertices == nullptr || vertexNum <= 0)
    {
        std::cerr << "Mesh::SetupVAO() failed: vertices is null or empty." << std::endl;
        return;
    }

    GLuint tmp_vao, tmp_vbo;

    /*
     * glGenVertexArrays 是 OpenGL 中的一个函数，用于生成一个或多个顶点数组对象 (VAO, Vertex Array Object)。

     * VAO 是一种容器对象，它存储所有与顶点属性相关的状态。VAO 记录了顶点属性的配置以及用于绘制的顶点缓冲对象的绑定情况。
     * VAO 是一个非常方便的工具，可以帮助管理顶点属性配置。只需要配置一次 VAO，然后在绘制时绑定相应的 VAO，就能恢复之前配置好的顶点属性状态。

     * glGenVertexArrays 是生成 VAO 的关键函数，它生成一个或多个 VAO 并返回其 ID。VAO 是一个强大的工具，用于管理与顶点属性相关的状态。
     * 通过使用 VAO，可以简化顶点属性的配置过程，并在绘制时快速切换不同的顶点属性配置。这对于提高渲染性能和代码可维护性非常有帮助。
    */
    glGenVertexArrays(1, &tmp_vao);

    /*
     * 在OpenGL中，glGenBuffers函数的作用是分配一个"Buffer Object"的名称。"Buffer Object"代表了一个可以被应用程序和GPU（图形处理单元）访问的内存块。
     * 当调用glGenBuffers时，它实际上并没有创建任何东西。它只是返回一组当前未被用作缓冲区名称的整数。实际的’对象’并不会在调用glBindBuffer之前被创建。
     * 因此，glGenBuffers实际上并不是必需的，它只是作为一个方便函数，提供一个未使用的整数。
     * 所以，如果只是创建了一组随机的整数，只要它们之间没有重叠，就可以将其作为缓冲区列表，而无需调用glGenBuffers。
    */
    glGenBuffers(1, &tmp_vbo);

    /*
     * glBindVertexArray 是 OpenGL 中的一个函数，用于绑定一个顶点数组对象 (VAO, Vertex Array Object)。
     * 绑定 VAO 是一个关键步骤，因为它使得所有后续的顶点属性配置和顶点缓冲对象 (VBO, Vertex Buffer Object) 绑定都关联到这个 VAO 上。
     * 当一个 VAO 被绑定后，所有与顶点数组相关的状态改变都会被存储在这个 VAO 中。

     * bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    */
    glBindVertexArray(tmp_vao);

    /*
     * 在OpenGL中，glBindBuffer函数用于将一个缓冲区对象（Buffer Object）绑定到一个指定的缓冲区绑定点。
     * 这个函数接受两个参数：一个是目标（target），另一个是缓冲区对象的名称（buffer）。

     * 当调用glBindBuffer函数时，以下几件事情会发生：
     *  1. 如果这个缓冲区对象之前没有被初始化（也就是说，这是第一次绑定这个缓冲区对象），那么OpenGL会为其分配内存。
     *  2. 这个缓冲区对象会被设置为当前活动的缓冲区对象。这意味着，之后对指定目标的所有操作都会影响到这个缓冲区对象。
     *  3. 如果之后调用了例如glBufferData或glBufferSubData等函数，这些函数会改变当前活动的缓冲区对象的数据。

     * GL_ARRAY_BUFFER：用于顶点属性数据，如顶点坐标、法线、颜色等。
    */
    glBindBuffer(GL_ARRAY_BUFFER, tmp_vbo);

    /*
     * glBufferData函数用于创建并初始化一个缓冲区对象的数据存储。

     * 这个函数接受四个参数：目标（target）、大小（size）、数据（data）和用途（usage）。
     *  目标（target）：这个参数定义了缓冲区对象的类型。例如，GL_ARRAY_BUFFER表示顶点数据，GL_ELEMENT_ARRAY_BUFFER表示索引数据等。
     *  大小（size）：这个参数指定了缓冲区对象的大小，以字节为单位。
     *  数据（data）：这个参数是一个指向希望存储在缓冲区对象中的数据的指针。如果这个参数不为NULL，那么缓冲区对象将会被初始化为这个指针指向的数据。
     *  用途（usage）：这个参数给出了缓冲区对象的预期使用模式。例如，GL_STATIC_DRAW表示数据将会被修改一次，但被读取多次。

     * 当调用glBufferData函数时，以下几件事情会发生：
     *  1. 如果之前已经存在一个数据存储，那么这个数据存储会被删除。
     *  2. 一个新的数据存储会被创建，大小和用途由参数指定。
     *  3. 如果数据参数不为NULL，那么新的数据存储会被初始化为这个参数指向的数据。
     *  4. glBufferData函数会创建一个数据在“快速内存”（即GPU可以直接访问的RAM）中的工作副本，而不是每次调用glDrawElements或glDrawArrays时都需要在内存域（即CPU→GPU）之间传输顶点属性。
    */
    glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    /*
     * 在OpenGL中,glVertexAttribPointer函数是用于设置顶点属性数据的重要函数。
     * 它的作用是将顶点数据(如位置、颜色、法线等)与指定的属性位置关联起来,以便GPU能够正确读取和处理这些数据。
     * 这个函数通常与顶点缓冲对象 (VBO) 一起使用，用于设置顶点着色器中顶点属性的数据来源。

     * 这个函数接受六个参数：索引（index）、大小（size）、类型（type）、是否标准化（normalized）、步长（stride）和指针（pointer）。
     *  index：指定要修改的顶点属性的索引。在顶点着色器中，这个索引与location是通过 layout (location = x) 指定的。例如，如果顶点着色器中有 layout (location = 0) in vec3 position;，那么这个索引就是 0。
     *  size：指定每个顶点属性的分量数量。例如，位置通常有3个分量（x, y, z），颜色可能有4个分量（r, g, b, a）。
     *  type：指定数组中每个组件的数据类型。常用的类型有GL_FLOAT、GL_INT、GL_UNSIGNED_INT等。
     *  normalized：如果设置为 GL_TRUE，整数类型的数值在被访问时会被映射到 [0,1] (无符号) 或 [-1,1] (有符号) 范围内。如果设置为 GL_FALSE，则直接作为浮点数处理。
     *  stride：指定连续顶点属性之间的字节偏移量。如果为 0，则表示顶点属性是紧密排列的（即无间隔）。这个参数用于在顶点缓冲对象中正确定位每个顶点的属性。
     *  pointer：指定顶点属性数组中第一个组件的字节偏移量。这个指针是从缓冲对象数据的起始位置开始计算的。
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

    /*
     * 在现代 OpenGL 中，顶点数据通常通过顶点缓冲对象 (VBO) 传递到 GPU，而这些数据可以包含多个不同的属性（如位置、颜色、法线、纹理坐标等）。
     * 然而，为了使这些数据在绘制时被使用，必须启用相应的顶点属性数组。这就是 glEnableVertexAttribArray 的作用。

     * 调用这个函数后,当使用glDrawArrays或glDrawElements进行渲染时,对应位置的顶点属性数据将被GPU读取和处理,用于顶点着色器的计算。
     * 如果没有调用glEnableVertexAttribArray,即使我们使用glVertexAttribPointer设置了顶点属性数据的存储方式,GPU也不会读取和使用这些数据。
     * 需要注意的是,在设置完顶点属性指针后,默认情况下所有顶点属性数组都是禁用的,必须手动调用glEnableVertexAttribArray来启用需要使用的属性。
    */
    glEnableVertexAttribArray(0);

    /*
     * note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object,
     * so afterwards we can safely unbind
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*
     * You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
     * Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    */
    glBindVertexArray(0);

    vao = tmp_vao;
    vbo = tmp_vbo;
}

void Mesh::SetupVAO_Elements(const GLfloat *vertices, const int vertexNum, const GLuint *indices, const int indexNum)
{
    if (vertices == nullptr || vertexNum <= 0)
    {
        std::cerr << "Mesh::SetupVAO_Elements() failed: vertices is null or empty." << std::endl;
        return;
    }

    if (indices == nullptr || indexNum <= 0)
    {
        std::cerr << "Mesh::SetupVAO_Elements() failed: indices is null or empty." << std::endl;
        return;
    }

    GLuint tmp_vao, tmp_vbo, tmp_ebo;

    /*
     * glGenVertexArrays 是 OpenGL 中的一个函数，用于生成一个或多个顶点数组对象 (VAO, Vertex Array Object)。

     * VAO 是一种容器对象，它存储所有与顶点属性相关的状态。VAO 记录了顶点属性的配置以及用于绘制的顶点缓冲对象的绑定情况。
     * VAO 是一个非常方便的工具，可以帮助管理顶点属性配置。只需要配置一次 VAO，然后在绘制时绑定相应的 VAO，就能恢复之前配置好的顶点属性状态。

     * glGenVertexArrays 是生成 VAO 的关键函数，它生成一个或多个 VAO 并返回其 ID。VAO 是一个强大的工具，用于管理与顶点属性相关的状态。
     * 通过使用 VAO，可以简化顶点属性的配置过程，并在绘制时快速切换不同的顶点属性配置。这对于提高渲染性能和代码可维护性非常有帮助。
    */
    glGenVertexArrays(1, &tmp_vao);

    /*
     * 在OpenGL中，glGenBuffers函数的作用是分配一个"Buffer Object"的名称。"Buffer Object"代表了一个可以被应用程序和GPU（图形处理单元）访问的内存块。
     * 当调用glGenBuffers时，它实际上并没有创建任何东西。它只是返回一组当前未被用作缓冲区名称的整数。实际的’对象’并不会在调用glBindBuffer之前被创建。
     * 因此，glGenBuffers实际上并不是必需的，它只是作为一个方便函数，提供一个未使用的整数。
     * 所以，如果只是创建了一组随机的整数，只要它们之间没有重叠，就可以将其作为缓冲区列表，而无需调用glGenBuffers。
    */
    glGenBuffers(1, &tmp_vbo);
    glGenBuffers(1, &tmp_ebo);

    /*
     * glBindVertexArray 是 OpenGL 中的一个函数，用于绑定一个顶点数组对象 (VAO, Vertex Array Object)。
     * 绑定 VAO 是一个关键步骤，因为它使得所有后续的顶点属性配置和顶点缓冲对象 (VBO, Vertex Buffer Object) 绑定都关联到这个 VAO 上。
     * 当一个 VAO 被绑定后，所有与顶点数组相关的状态改变都会被存储在这个 VAO 中。

     * bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    */
    glBindVertexArray(tmp_vao);

    /*
     * 在OpenGL中，glBindBuffer函数用于将一个缓冲区对象（Buffer Object）绑定到一个指定的缓冲区绑定点。
     * 这个函数接受两个参数：一个是目标（target），另一个是缓冲区对象的名称（buffer）。

     * 当调用glBindBuffer函数时，以下几件事情会发生：
     *  1. 如果这个缓冲区对象之前没有被初始化（也就是说，这是第一次绑定这个缓冲区对象），那么OpenGL会为其分配内存。
     *  2. 这个缓冲区对象会被设置为当前活动的缓冲区对象。这意味着，之后对指定目标的所有操作都会影响到这个缓冲区对象。
     *  3. 如果之后调用了例如glBufferData或glBufferSubData等函数，这些函数会改变当前活动的缓冲区对象的数据。

     * GL_ARRAY_BUFFER：用于顶点属性数据，如顶点坐标、法线、颜色等。
    */
    glBindBuffer(GL_ARRAY_BUFFER, tmp_vbo);

    /*
     * glBufferData函数用于创建并初始化一个缓冲区对象的数据存储。

     * 这个函数接受四个参数：目标（target）、大小（size）、数据（data）和用途（usage）。
     *  目标（target）：这个参数定义了缓冲区对象的类型。例如，GL_ARRAY_BUFFER表示顶点数据，GL_ELEMENT_ARRAY_BUFFER表示索引数据等。
     *  大小（size）：这个参数指定了缓冲区对象的大小，以字节为单位。
     *  数据（data）：这个参数是一个指向希望存储在缓冲区对象中的数据的指针。如果这个参数不为NULL，那么缓冲区对象将会被初始化为这个指针指向的数据。
     *  用途（usage）：这个参数给出了缓冲区对象的预期使用模式。例如，GL_STATIC_DRAW表示数据将会被修改一次，但被读取多次。

     * 当调用glBufferData函数时，以下几件事情会发生：
     *  1. 如果之前已经存在一个数据存储，那么这个数据存储会被删除。
     *  2. 一个新的数据存储会被创建，大小和用途由参数指定。
     *  3. 如果数据参数不为NULL，那么新的数据存储会被初始化为这个参数指向的数据。
     *  4. glBufferData函数会创建一个数据在“快速内存”（即GPU可以直接访问的RAM）中的工作副本，而不是每次调用glDrawElements或glDrawArrays时都需要在内存域（即CPU→GPU）之间传输顶点属性。
    */
    glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    /*
     * 在OpenGL中，glBindBuffer函数用于将一个缓冲区对象（Buffer Object）绑定到一个指定的缓冲区绑定点。
     * 这个函数接受两个参数：一个是目标（target），另一个是缓冲区对象的名称（buffer）。

     * 当调用glBindBuffer函数时，以下几件事情会发生：
     *  1. 如果这个缓冲区对象之前没有被初始化（也就是说，这是第一次绑定这个缓冲区对象），那么OpenGL会为其分配内存。
     *  2. 这个缓冲区对象会被设置为当前活动的缓冲区对象。这意味着，之后对指定目标的所有操作都会影响到这个缓冲区对象。
     *  3. 如果之后调用了例如glBufferData或glBufferSubData等函数，这些函数会改变当前活动的缓冲区对象的数据。

     * GL_ELEMENT_ARRAY_BUFFER：用于存储索引数据（Index Data）。
    */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmp_ebo);

    /*
     * glBufferData函数用于创建并初始化一个缓冲区对象的数据存储。

     * 这个函数接受四个参数：目标（target）、大小（size）、数据（data）和用途（usage）。
     *  目标（target）：这个参数定义了缓冲区对象的类型。例如，GL_ARRAY_BUFFER表示顶点数据，GL_ELEMENT_ARRAY_BUFFER表示索引数据等。
     *  大小（size）：这个参数指定了缓冲区对象的大小，以字节为单位。
     *  数据（data）：这个参数是一个指向希望存储在缓冲区对象中的数据的指针。如果这个参数不为NULL，那么缓冲区对象将会被初始化为这个指针指向的数据。
     *  用途（usage）：这个参数给出了缓冲区对象的预期使用模式。例如，GL_STATIC_DRAW表示数据将会被修改一次，但被读取多次。

     * 当调用glBufferData函数时，以下几件事情会发生：
     *  1. 如果之前已经存在一个数据存储，那么这个数据存储会被删除。
     *  2. 一个新的数据存储会被创建，大小和用途由参数指定。
     *  3. 如果数据参数不为NULL，那么新的数据存储会被初始化为这个参数指向的数据。
     *  4. glBufferData函数会创建一个数据在“快速内存”（即GPU可以直接访问的RAM）中的工作副本，而不是每次调用glDrawElements或glDrawArrays时都需要在内存域（即CPU→GPU）之间传输顶点属性。
    */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum * sizeof(GLuint), indices, GL_STATIC_DRAW);

    /*
     * 在OpenGL中,glVertexAttribPointer函数是用于设置顶点属性数据的重要函数。
     * 它的作用是将顶点数据(如位置、颜色、法线等)与指定的属性位置关联起来,以便GPU能够正确读取和处理这些数据。
     * 这个函数通常与顶点缓冲对象 (VBO) 一起使用，用于设置顶点着色器中顶点属性的数据来源。

     * 这个函数接受六个参数：索引（index）、大小（size）、类型（type）、是否标准化（normalized）、步长（stride）和指针（pointer）。
     *  index：指定要修改的顶点属性的索引。在顶点着色器中，这个索引与location是通过 layout (location = x) 指定的。例如，如果顶点着色器中有 layout (location = 0) in vec3 position;，那么这个索引就是 0。
     *  size：指定每个顶点属性的分量数量。例如，位置通常有3个分量（x, y, z），颜色可能有4个分量（r, g, b, a）。
     *  type：指定数组中每个组件的数据类型。常用的类型有GL_FLOAT、GL_INT、GL_UNSIGNED_INT等。
     *  normalized：如果设置为 GL_TRUE，整数类型的数值在被访问时会被映射到 [0,1] (无符号) 或 [-1,1] (有符号) 范围内。如果设置为 GL_FALSE，则直接作为浮点数处理。
     *  stride：指定连续顶点属性之间的字节偏移量。如果为 0，则表示顶点属性是紧密排列的（即无间隔）。这个参数用于在顶点缓冲对象中正确定位每个顶点的属性。
     *  pointer：指定顶点属性数组中第一个组件的字节偏移量。这个指针是从缓冲对象数据的起始位置开始计算的。
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);

    /*
     * 在现代 OpenGL 中，顶点数据通常通过顶点缓冲对象 (VBO) 传递到 GPU，而这些数据可以包含多个不同的属性（如位置、颜色、法线、纹理坐标等）。
     * 然而，为了使这些数据在绘制时被使用，必须启用相应的顶点属性数组。这就是 glEnableVertexAttribArray 的作用。

     * 调用这个函数后,当使用glDrawArrays或glDrawElements进行渲染时,对应位置的顶点属性数据将被GPU读取和处理,用于顶点着色器的计算。
     * 如果没有调用glEnableVertexAttribArray,即使我们使用glVertexAttribPointer设置了顶点属性数据的存储方式,GPU也不会读取和使用这些数据。
     * 需要注意的是,在设置完顶点属性指针后,默认情况下所有顶点属性数组都是禁用的,必须手动调用glEnableVertexAttribArray来启用需要使用的属性。
    */
    glEnableVertexAttribArray(0);

    // 设置并启用顶点的颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    /*
     * 此处不要解绑ebo！

     * GL_ELEMENT_ARRAY_BUFFER 的行为稍有不同。EBO 是用于存储顶点索引数据的缓冲区对象，它们同样与当前的 VAO 绑定。但不同的是，EBO 的绑定状态是存储在 VAO 中的，而不是全局的状态。
     * 当绑定或解绑一个缓冲区对象到 GL_ELEMENT_ARRAY_BUFFER 时，它的状态会存储在当前绑定的 VAO 中。
     * 如果尝试解绑 GL_ELEMENT_ARRAY_BUFFER，实际上是在对当前绑定的 VAO 做操作。这通常是不必要的，因为希望 VAO 记录当前使用的 EBO。

     * 当调用glBindBuffer(GL_ARRAY_BUFFER, 0)时，实际上是在解绑当前绑定到GL_ARRAY_BUFFER目标的缓冲区。
     * 这是可以的，因为当调用glVertexAttribPointer时，VAO会存储与GL_ARRAY_BUFFER相关的状态。
     * 因此，即使我们解绑了GL_ARRAY_BUFFER，VAO仍然知道如何访问之前绑定的缓冲区的数据。

     * 然而，对于GL_ELEMENT_ARRAY_BUFFER，情况就不同了。
     * 当调用glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)时，试图解绑当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的索引缓冲区。
     * 但是，VAO的状态信息中包含了GL_ELEMENT_ARRAY_BUFFER的绑定状态。
     * 因此，如果我们试图在解绑VAO之前解绑GL_ELEMENT_ARRAY_BUFFER，OpenGL将无法找到正确的索引缓冲区，从而无法正确渲染图形。

     * 总的来说，GL_ARRAY_BUFFER和GL_ELEMENT_ARRAY_BUFFER在VAO中的行为差异主要是由于OpenGL的历史原因和设计决策。

     * A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER. 
     * This also means it stores its unbind calls so make sure you don't unbind the element array buffer before unbinding your VAO, 
     * otherwise it doesn't have an EBO configured.
    */
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /*
     * note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object,
     * so afterwards we can safely unbind
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*
     * You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
     * Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    */
    glBindVertexArray(0);

    vao = tmp_vao;
    vbo = tmp_vbo;
    ebo = tmp_ebo;
}