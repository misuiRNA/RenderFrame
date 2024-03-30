#ifndef _HEAD_FLAG_RENDERDATA_H
#define _HEAD_FLAG_RENDERDATA_H

#include <vector>
#include "BaseDefine.h"
#include <map>

struct RenderData {
    RenderData();
    RenderData(const std::map<std::string, int>& attrNameMap, const std::map<std::string, int>& textureSlotNameMap);
    ~RenderData();

    void setVertices(unsigned int index, unsigned int vertexSize, const std::vector<float>& vertices);
    void setVertices(const std::string& name, unsigned int vertexSize, const std::vector<float>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
    void setTexture(unsigned int slotIndex, unsigned int width, unsigned int height, const unsigned char* imageData, unsigned int format);
    void setTexture(const std::string& name, unsigned int width, unsigned int height, const unsigned char* imageData, unsigned int format);

    void draw();
    void useTextures();

private:
    unsigned int _VAOId;

    int _vertexCount;
    int _indexCount;

    const std::map<std::string, int>& _attrNameMap;
    const std::map<std::string, int>& _textureSlotNameMap;
    std::map<int, unsigned int> _textureMap;
};

#endif // _HEAD_FLAG_RENDERDATA_H
