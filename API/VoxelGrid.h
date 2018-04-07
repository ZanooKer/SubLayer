#ifndef __O3DP_VOXEL_GRID_H__
#define __O3DP_VOXEL_GRID_H__

#include "IntTypes.h"
#include "BBox3d.h"
#include "Vec3i.h"
#include "Enums.h"
#include "VoxelGridSlice.h"

#include <assert.h>
#include <cstring>
#include <utility>
#include <vector>

class O3DPVoxelGrid {
  public:
    O3DPVoxelGrid(const MfVec3i &numVoxels,
                  const MfBBox3d &bbox,
                  int numMaterials,
                  O3DPAuxDataType dataType) :
        _numVoxels(numVoxels),
        _bbox(bbox),
        _numMaterials(numMaterials),
        _auxDataType(dataType)
    {
        _XY = numVoxels[0] * numVoxels[1];
        const size_t XYZ = _XY * _numVoxels[2];
        _voxels.resize(XYZ, 0);
        if (_auxDataType != O3DP_AUX_DATA_TYPE_NONE) {
            _auxData.resize(GetAuxDataSize() * XYZ, 0);
        }
    }

    const MfVec3i & GetNumVoxels() const {
        return _numVoxels;
    }

    const MfVec3i & GetSize() const {
        return _numVoxels;
    }

    const MfBBox3d & GetBBox() const {
        return _bbox;
    }

    uint32_t GetNumMaterials() const {
        return _numMaterials;
    }

    void SetMaterial(const MfVec3i &xyz, uint8_t matIndex) {
        SetMaterial(xyz[0], xyz[1], xyz[2], matIndex);
    }

    void SetMaterial(int x, int y, int z, uint8_t matIndex) {
        size_t index = _GetIndex(x, y, z);
        _voxels[index] = matIndex;
        // Keep track of what materials we write into the grid
        // by selecting the maximum index we see. We subtract 2 because
        // 0 is void and 1 is support. Void (0) and support (1) do 
        // not count towards number of materials. 
        if (matIndex >= 2) {
            _numMaterials = std::max(_numMaterials, uint32_t(matIndex) - 2);
        }
    }

    uint8_t GetMaterial(int x, int y, int z) const {
        size_t index = _GetIndex(x, y, z);
        return _voxels[index];
    }

    void AppendEmpty(int numSlices);

    void AddLeftMargin(int voxels);

    void AddRightMargin(int voxels);

    void AddTopMargin(int voxels);

    void AddBottomMargin(int voxels);

    void FlipX();

    void FlipY();

    std::vector<int> ReplaceMaterials(
        const std::vector<std::pair<uint8_t, uint8_t> > &replacements);

    void ReplaceMaterials(uint8_t oldMaterial, uint8_t newMaterial) {
        std::vector<std::pair<uint8_t, uint8_t> > replacement;
        replacement.push_back(std::make_pair(oldMaterial, newMaterial));
        ReplaceMaterials(replacement);
    }

    void SetAuxData(const MfVec3i &xyz, const void *data) {
        SetAuxData(xyz[0], xyz[1], xyz[2], data);
    }

    void SetAuxData(int x, int y, int z, const void *data) {
        if (!_auxData.empty()) {
            size_t index = _GetIndex(x, y, z);
            const size_t dataSize = O3DPAuxDataTypeSize[_auxDataType];
            std::memcpy(&_auxData[index * dataSize], data, dataSize);
        }
    }

    const void * GetAuxData(const MfVec3i &xyz) const {
        return GetAuxData(xyz[0], xyz[1], xyz[2]);
    }

    const void * GetAuxData(int x, int y, int z) const {
        if (_auxData.empty()) {
            return NULL;
        } else {
            size_t index = _GetIndex(x, y, z);
            const size_t dataSize = O3DPAuxDataTypeSize[_auxDataType];
            return &_auxData[index * dataSize];
        }
    }

    O3DPAuxDataType GetAuxDataType() const {
        return _auxDataType;
    }

    size_t GetAuxDataSize() const {
        return O3DPAuxDataTypeSize[_auxDataType];
    }

    void CopyDataFrom(O3DPVoxelGrid *grid, int x, int y, int z) {
        SetMaterial(x, y, z, grid->GetMaterial(x, y, z));
        if (_auxDataType != O3DP_AUX_DATA_TYPE_NONE && 
            _auxDataType == grid->GetAuxDataType()) {
            SetAuxData(x, y, z, grid->GetAuxData(x, y, z));
        }
    }

  protected:
    friend class O3DPReader;
    friend class O3DPWriter;

    typedef std::vector<uint8_t> _VoxelData;
    typedef std::vector<uint8_t> _VoxelAuxData;

    uint8_t * _GetData() {
        return &_voxels[0];
    }

    const uint8_t * _GetData() const {
        return &_voxels[0];
    }

    uint8_t * _GetSliceData(int z) {
        size_t index = _GetIndex(0, 0, z);
        const size_t dataSize = sizeof(uint8_t);
        return &_voxels[index * dataSize];
    }

    const uint8_t * _GetSliceData(int z) const {
        size_t index = _GetIndex(0, 0, z);
        const size_t dataSize = sizeof(uint8_t);
        return &_voxels[index * dataSize];
    }

    const size_t _GetVoxelSize() const {
        return sizeof(_VoxelData::value_type);
    }

    uint8_t * _GetAuxData() {
        if (_auxData.empty()) {
            return NULL;
        } else {
            return &_auxData[0];
        }
    }

    const uint8_t * _GetAuxData() const {
        if (_auxData.empty()) {
            return NULL;
        } else {
            return &_auxData[0];
        }
    }

    uint8_t * _GetSliceAuxData(int z) {
        if (_auxData.empty()) {
            return NULL;
        } else {
            size_t index = _GetIndex(0, 0, z);
            const size_t dataSize = O3DPAuxDataTypeSize[_auxDataType];
            return &_auxData[index * dataSize];
        }
    }

    const uint8_t * _GetSliceAuxData(int z) const {
        if (_auxData.empty()) {
            return NULL;
        } else {
            size_t index = _GetIndex(0, 0, z);
            const size_t dataSize = O3DPAuxDataTypeSize[_auxDataType];
            return &_auxData[index * dataSize];
        }
    }
    
    inline size_t _GetIndex(int x, int y, int z) const {
        assert(x < _numVoxels[0]);
        assert(y < _numVoxels[1]);
        assert(z < _numVoxels[2]);
        return z * _XY + y * _numVoxels[0] + x;
    }

  private:
    MfVec3i _numVoxels;
    _VoxelData _voxels;

    size_t _XY;
    MfBBox3d _bbox;
    uint32_t _numMaterials;
    O3DPAuxDataType _auxDataType;
    _VoxelAuxData _auxData;
};

template <class T>
class O3DPAuxVoxelGrid : public O3DPVoxelGrid {
  public:
    O3DPAuxVoxelGrid(const MfVec3i &numVoxels,
                     const MfBBox3d &bbox,
                     int numMaterials) :
        O3DPVoxelGrid(numVoxels, bbox, numMaterials, O3DPGetAuxDataType<T>())
    {
    }

    void SetAux(const MfVec3i &xyz, const T &data) {
        SetAuxData(xyz, reinterpret_cast<const uint8_t*>(&data));
    }

    void SetAux(int x, int y, int z, const T &data) {
        SetAuxData(x, y, z, reinterpret_cast<const uint8_t*>(&data));
    }

    const T & GetAux(const MfVec3i &xyz) const {
        return *reinterpret_cast<const T*>(GetAuxData(xyz));
    }

    const T & GetAux(int x, int y, int z) const {
        return *reinterpret_cast<const T*>(GetAuxData(x, y, z));
    }
};

#endif // __O3DP_VOXEL_GRID_H__
