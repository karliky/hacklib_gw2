#include "main.h"
#include <thread>


static const int CIRCLE_RES = 64;
static const hl::VertexBuffer *vbCircle;
static const hl::VertexBuffer *vbRect;

bool InitEsp()
{
    auto pDrawer = GetMain()->GetDrawer(false);
    if (!pDrawer) return false;

    std::vector<hl::VERTEX_3D_COL> rectVerts = {
        { -1, -1, 0 },
        {  1, -1, 0 },
        {  1,  1, 0 },
        { -1,  1, 0 },
        { -1, -1, 0 }
    };

    vbRect = pDrawer->allocVertexBuffer(rectVerts);
    if (!vbRect)
        return false;


    std::vector<hl::VERTEX_3D_COL> verts;
    verts.resize(CIRCLE_RES+1);
    for (size_t i = 0; i < verts.size(); i++) {
        verts[i].x = cos(D3DX_PI*((CIRCLE_RES-i)/(CIRCLE_RES/2.0f)));
        verts[i].y = sin(D3DX_PI*((CIRCLE_RES-i)/(CIRCLE_RES/2.0f)));
        verts[i].z = 0;
    }

    vbCircle = pDrawer->allocVertexBuffer(verts);
    if (vbCircle)
        return true;

    //g_pCon->printf("[InitEsp] could not init esp data\n");
    return false;
}


void GW2LIB::EnableEsp(void (*cbRender)())
{
    GetMain()->SetRenderCallback(cbRender);
}


void GW2LIB::DrawLine(float x, float y, float x2, float y2, DWORD color)
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer) {
        pDrawer->drawLine(x, y, x2, y2, color);
    }
}

void GW2LIB::DrawLineProjected(Vector3 pos1, Vector3 pos2, DWORD color)
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer) {
        pDrawer->drawLineProjected(D3DXVECTOR3(pos1.x,pos1.y,pos1.z), D3DXVECTOR3(pos2.x,pos2.y,pos2.z), color);
    }
}

void GW2LIB::DrawRect(float x, float y, float w, float h, DWORD color)
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer) {
        pDrawer->drawRect(x, y, w, h, color);
    }
}

void GW2LIB::DrawRectFilled(float x, float y, float w, float h, DWORD color)
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer) {
        pDrawer->drawRectFilled(x, y, w, h, color);
    }
}

void GW2LIB::DrawCircle(float mx, float my, float r, DWORD color)
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer) {
        pDrawer->drawCircle(mx, my, r, color);
    }
}

void GW2LIB::DrawCircleFilled(float mx, float my, float r,  DWORD color)
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer) {
        pDrawer->drawCircleFilled(mx, my, r, color);
    }
}

void GW2LIB::DrawCircleProjected(Vector3 pos, float r, DWORD color)
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer) {
        D3DXMATRIX scale, translate;
        D3DXMatrixScaling(&scale, r, r, r);
        D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
        pDrawer->drawPrimitive(vbCircle, nullptr, D3DPT_LINESTRIP, scale*translate, color);
    }
}

void GW2LIB::DrawCircleFilledProjected(Vector3 pos, float r, DWORD color)
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer) {
        D3DXMATRIX scale, translate;
        D3DXMatrixScaling(&scale, r, r, r);
        D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
        pDrawer->drawPrimitive(vbCircle, nullptr, D3DPT_TRIANGLEFAN, scale*translate, color);
    }
}

void GW2LIB::DrawRectProjected(Vector3 pos, float x, float y, float rot, DWORD color)
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer) {
        D3DXMATRIX scale, translate, rotationZ;
        D3DXMatrixScaling(&scale, x, y, 0);
        D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
        D3DXMatrixRotationZ(&rotationZ, rot);
        pDrawer->drawPrimitive(vbRect, nullptr, D3DPT_LINESTRIP, scale*rotationZ*translate, color);
    }
}

void GW2LIB::DrawRectFilledProjected(Vector3 pos, float x, float y, float rot, DWORD color)
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer) {
        D3DXMATRIX scale, translate, rotationZ;
        D3DXMatrixScaling(&scale, x, y, 0);
        D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
        D3DXMatrixRotationZ(&rotationZ, rot);
        pDrawer->drawPrimitive(vbRect, nullptr, D3DPT_TRIANGLEFAN, scale*rotationZ*translate, color);
    }
}


bool GW2LIB::WorldToScreen(Vector3 in, float *outX, float *outY)
{
    const auto pDrawer = GetMain()->GetDrawer(false);
    if (pDrawer) {
        hl::Vec3 out(0);
        pDrawer->project(hl::Vec3(in.x,in.y,in.z), out);
        if (pDrawer->isInfrontCam(out)) {
            *outX = out.x;
            *outY = out.y;
            return true;
        }
    }
    return false;
}


float GW2LIB::GetWindowWidth()
{
    const auto pDrawer = GetMain()->GetDrawer(false);
    if (pDrawer) {
        return pDrawer->getWidth();
    }
    return 0;
}

float GW2LIB::GetWindowHeight()
{
    const auto pDrawer = GetMain()->GetDrawer(false);
    if (pDrawer) {
        return pDrawer->getHeight();
    }
    return 0;
}


GW2LIB::Texture::Texture()
{
    m_ptr = nullptr;
}

bool GW2LIB::Texture::Init(std::string file)
{
    auto pDrawer = GetMain()->GetDrawer(false);
    if (pDrawer) {
        m_ptr = reinterpret_cast<const void*>(pDrawer->allocTexture(file.c_str()));
        if (m_ptr)
            return true;
    }
    return false;
}

bool GW2LIB::Texture::Init(const void *buffer, size_t size)
{
    auto pDrawer = GetMain()->GetDrawer(false);
    if (pDrawer) {
        m_ptr = reinterpret_cast<const void*>(pDrawer->allocTexture(buffer, size));
        if (m_ptr)
            return true;
    }
    return false;
}

void GW2LIB::Texture::Draw(float x, float y, float w, float h) const
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer && m_ptr)
        pDrawer->drawTexture(reinterpret_cast<const hl::Texture*>(m_ptr), x, y, w, h);
}


GW2LIB::Font::Font()
{
    m_ptr = nullptr;
}

bool GW2LIB::Font::Init(int size, std::string name, bool bold)
{
    auto pDrawer = GetMain()->GetDrawer(false);
    if (pDrawer) {
        m_ptr = reinterpret_cast<const void*>(pDrawer->allocFont(name, size, bold));
        if (m_ptr)
            return true;
    }
    return false;
}

void GW2LIB::Font::Draw(float x, float y, DWORD color, std::string format, va_list vl) const
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer && m_ptr)
        pDrawer->drawFont(reinterpret_cast<const hl::Font*>(m_ptr), x, y, color, format, vl);
}

void GW2LIB::Font::Draw(float x, float y, DWORD color, std::string format, ...) const
{
    va_list vl;
    va_start(vl, format);
    Draw(x, y, color, format, vl);
    va_end(vl);
}

GW2LIB::Vector2 GW2LIB::Font::TextInfo(std::string str) const {
    D3DXVECTOR2 vec = { 0, 0 };
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer && m_ptr)
        vec = pDrawer->textInfo(reinterpret_cast<const hl::Font*>(m_ptr), str);

    return { vec.x, vec.y };
}


struct PrimitiveDiffuseMesh {
    const hl::VertexBuffer *vertBuffer;
    const hl::IndexBuffer *indBuffer;
    D3DPRIMITIVETYPE type;
    std::vector<D3DXMATRIX> transforms;
};

GW2LIB::PrimitiveDiffuse::PrimitiveDiffuse()
{
    m_ptr = nullptr;
}

GW2LIB::PrimitiveDiffuse::~PrimitiveDiffuse()
{
    delete m_ptr;
}

bool GW2LIB::PrimitiveDiffuse::Init(std::vector<std::pair<Vector3,DWORD>> vertices, std::vector<unsigned int> indices, bool triangleStrip)
{
    if (m_ptr) return false;
    auto pDrawer = GetMain()->GetDrawer(false);
    if (pDrawer) {
        m_ptr = new PrimitiveDiffuseMesh;

        std::vector<hl::VERTEX_3D_DIF> verts;
        verts.resize(vertices.size());
        for (size_t i = 0; i < vertices.size(); i++) {
            verts[i].x = vertices[i].first.x;
            verts[i].y = vertices[i].first.y;
            verts[i].z = vertices[i].first.z;
            verts[i].color = vertices[i].second;
        }
        m_ptr->vertBuffer = pDrawer->allocVertexBuffer(verts);
        if (!m_ptr->vertBuffer) {
            delete m_ptr;
            m_ptr = nullptr;
            return false;
        }

        m_ptr->indBuffer = nullptr;
        if (indices.size()) {
            m_ptr->indBuffer = pDrawer->allocIndexBuffer(indices);
            if (!m_ptr->indBuffer) {
                delete m_ptr;
                m_ptr = nullptr;
                return false;
            }
        }

        if (triangleStrip)
            m_ptr->type = D3DPT_TRIANGLESTRIP;
        else
            m_ptr->type = D3DPT_TRIANGLELIST;

        return true;
    }
    return false;
}

void GW2LIB::PrimitiveDiffuse::SetTransforms(std::vector<Matrix4x4> transforms)
{
    if (m_ptr) {
        m_ptr->transforms.resize(transforms.size());
        for (size_t i = 0; i < transforms.size(); i++) {
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    m_ptr->transforms[i].m[x][y] = transforms[i].m[x][y];
                }
            }
        }
    }
}

void GW2LIB::PrimitiveDiffuse::AddTransform(Matrix4x4 transform)
{
    if (m_ptr) {
        D3DXMATRIX dxMatrix;
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                dxMatrix.m[x][y] = transform.m[x][y];
            }
        }
        m_ptr->transforms.push_back(dxMatrix);
    }
}

void GW2LIB::PrimitiveDiffuse::Draw() const
{
    const auto pDrawer = GetMain()->GetDrawer(true);
    if (pDrawer && m_ptr) {
        for (size_t i = 0; i < m_ptr->transforms.size(); i++) {
            pDrawer->drawPrimitive(m_ptr->vertBuffer, m_ptr->indBuffer, m_ptr->type, m_ptr->transforms[i]);
        }
    }
}