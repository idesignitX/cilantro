#pragma once

#include <cilantro/grid_accumulator.hpp>
#include <cilantro/accumulators.hpp>

namespace cilantro {
    template <typename ScalarT, ptrdiff_t EigenDim>
    class PointsGridDownsampler : public GridAccumulator<ScalarT,EigenDim,PointSumAccumulatorProxy<ScalarT,EigenDim>> {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        PointsGridDownsampler(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &points, ScalarT bin_size)
                : GridAccumulator<ScalarT,EigenDim,PointSumAccumulatorProxy<ScalarT,EigenDim>>(points, bin_size, PointSumAccumulatorProxy<ScalarT,EigenDim>(points))
        {}

        const PointsGridDownsampler& getDownsampledPoints(VectorSet<ScalarT,EigenDim> &ds_points, size_t min_points_in_bin = 1) const {
            ds_points.resize(this->data_map_.rows(), this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_points.col(ind++) = scale*this->bin_iterators_[k]->second.pointSum;
                }
            }

            ds_points.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }

        inline VectorSet<ScalarT,EigenDim> getDownsampledPoints(size_t min_points_in_bin = 1) const {
            VectorSet<ScalarT,EigenDim> ds_points;
            getDownsampledPoints(ds_points, min_points_in_bin);
            return ds_points;
        }
    };

    template <typename ScalarT, ptrdiff_t EigenDim>
    class PointsNormalsGridDownsampler : public GridAccumulator<ScalarT,EigenDim,PointNormalSumAccumulatorProxy<ScalarT,EigenDim>> {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        PointsNormalsGridDownsampler(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &points,
                                     const ConstVectorSetMatrixMap<ScalarT,EigenDim> &normals,
                                     ScalarT bin_size)
                : GridAccumulator<ScalarT,EigenDim,PointNormalSumAccumulatorProxy<ScalarT,EigenDim>>(points, bin_size, PointNormalSumAccumulatorProxy<ScalarT,EigenDim>(points, normals))
        {}

        const PointsNormalsGridDownsampler& getDownsampledPoints(VectorSet<ScalarT,EigenDim> &ds_points, size_t min_points_in_bin = 1) const {
            ds_points.resize(this->data_map_.rows(), this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_points.col(ind++) = scale*this->bin_iterators_[k]->second.pointSum;
                }
            }

            ds_points.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }

        inline VectorSet<ScalarT,EigenDim> getDownsampledPoints(size_t min_points_in_bin = 1) const {
            VectorSet<ScalarT,EigenDim> ds_points;
            getDownsampledPoints(ds_points, min_points_in_bin);
            return ds_points;
        }

        const PointsNormalsGridDownsampler& getDownsampledNormals(VectorSet<ScalarT,EigenDim> &ds_normals, size_t min_points_in_bin = 1) const {
            ds_normals.resize(this->data_map_.rows(), this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_normals.col(ind++) = (scale*this->bin_iterators_[k]->second.normalSum).normalized();
                }
            }

            ds_normals.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }

        inline VectorSet<ScalarT,EigenDim> getDownsampledNormals(size_t min_points_in_bin = 1) const {
            VectorSet<ScalarT,EigenDim> ds_normals;
            getDownsampledNormals(ds_normals, min_points_in_bin);
            return ds_normals;
        }

        const PointsNormalsGridDownsampler& getDownsampledPointsNormals(VectorSet<ScalarT,EigenDim> &ds_points,
                                                                        VectorSet<ScalarT,EigenDim> &ds_normals,
                                                                        size_t min_points_in_bin = 1) const
        {
            ds_points.resize(this->data_map_.rows(), this->grid_lookup_table_.size());
            ds_normals.resize(this->data_map_.rows(), this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_points.col(ind) = scale*this->bin_iterators_[k]->second.pointSum;
                    ds_normals.col(ind++) = (scale*this->bin_iterators_[k]->second.normalSum).normalized();
                }
            }

            ds_points.conservativeResize(Eigen::NoChange, ind);
            ds_normals.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }
    };

    template <typename ScalarT, ptrdiff_t EigenDim>
    class PointsColorsGridDownsampler : public GridAccumulator<ScalarT,EigenDim,PointColorSumAccumulatorProxy<ScalarT,EigenDim>> {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        PointsColorsGridDownsampler(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &points,
                                    const ConstVectorSetMatrixMap<float,3> &colors,
                                    ScalarT bin_size)
                : GridAccumulator<ScalarT,EigenDim,PointColorSumAccumulatorProxy<ScalarT,EigenDim>>(points, bin_size, PointColorSumAccumulatorProxy<ScalarT,EigenDim>(points, colors))
        {}

        const PointsColorsGridDownsampler& getDownsampledPoints(VectorSet<ScalarT,EigenDim> &ds_points, size_t min_points_in_bin = 1) const {
            ds_points.resize(this->data_map_.rows(), this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_points.col(ind++) = scale*this->bin_iterators_[k]->second.pointSum;
                }
            }

            ds_points.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }

        inline VectorSet<ScalarT,EigenDim> getDownsampledPoints(size_t min_points_in_bin = 1) const {
            VectorSet<ScalarT,EigenDim> ds_points;
            getDownsampledPoints(ds_points, min_points_in_bin);
            return ds_points;
        }

        const PointsColorsGridDownsampler& getDownsampledColors(VectorSet<float,3> &ds_colors, size_t min_points_in_bin = 1) const {
            ds_colors.resize(3, this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_colors.col(ind++) = scale*this->bin_iterators_[k]->second.colorSum;
                }
            }

            ds_colors.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }

        inline VectorSet<ScalarT,EigenDim> getDownsampledColors(size_t min_points_in_bin = 1) const {
            VectorSet<float,3> ds_colors;
            getDownsampledColors(ds_colors, min_points_in_bin);
            return ds_colors;
        }

        const PointsColorsGridDownsampler& getDownsampledPointsColors(VectorSet<ScalarT,EigenDim> &ds_points,
                                                                      VectorSet<float,3> &ds_colors,
                                                                      size_t min_points_in_bin = 1) const
        {
            ds_points.resize(this->data_map_.rows(), this->grid_lookup_table_.size());
            ds_colors.resize(3, this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_points.col(ind) = scale*this->bin_iterators_[k]->second.pointSum;
                    ds_colors.col(ind++) = scale*this->bin_iterators_[k]->second.colorSum;
                }
            }

            ds_points.conservativeResize(Eigen::NoChange, ind);
            ds_colors.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }
    };

    template <typename ScalarT, ptrdiff_t EigenDim>
    class PointsNormalsColorsGridDownsampler : public GridAccumulator<ScalarT,EigenDim,PointNormalColorSumAccumulatorProxy<ScalarT,EigenDim>> {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        PointsNormalsColorsGridDownsampler(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &points,
                                           const ConstVectorSetMatrixMap<ScalarT,EigenDim> &normals,
                                           const ConstVectorSetMatrixMap<float,3> &colors,
                                           ScalarT bin_size)
                : GridAccumulator<ScalarT,EigenDim,PointNormalColorSumAccumulatorProxy<ScalarT,EigenDim>>(points, bin_size, PointNormalColorSumAccumulatorProxy<ScalarT,EigenDim>(points, normals, colors))
        {}

        const PointsNormalsColorsGridDownsampler& getDownsampledPoints(VectorSet<ScalarT,EigenDim> &ds_points, size_t min_points_in_bin = 1) const {
            ds_points.resize(this->data_map_.rows(), this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_points.col(ind++) = scale*this->bin_iterators_[k]->second.pointSum;
                }
            }

            ds_points.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }

        inline VectorSet<ScalarT,EigenDim> getDownsampledPoints(size_t min_points_in_bin = 1) const {
            VectorSet<ScalarT,EigenDim> ds_points;
            getDownsampledPoints(ds_points, min_points_in_bin);
            return ds_points;
        }

        const PointsNormalsColorsGridDownsampler& getDownsampledNormals(VectorSet<ScalarT,EigenDim> &ds_normals, size_t min_points_in_bin = 1) const {
            ds_normals.resize(this->data_map_.rows(), this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_normals.col(ind++) = (scale*this->bin_iterators_[k]->second.normalSum).normalized();
                }
            }

            ds_normals.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }

        inline VectorSet<ScalarT,EigenDim> getDownsampledNormals(size_t min_points_in_bin = 1) const {
            VectorSet<ScalarT,EigenDim> ds_normals;
            getDownsampledNormals(ds_normals, min_points_in_bin);
            return ds_normals;
        }

        const PointsNormalsColorsGridDownsampler& getDownsampledColors(VectorSet<float,3> &ds_colors, size_t min_points_in_bin = 1) const {
            ds_colors.resize(3, this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_colors.col(ind++) = scale*this->bin_iterators_[k]->second.colorSum;
                }
            }

            ds_colors.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }

        inline VectorSet<ScalarT,EigenDim> getDownsampledColors(size_t min_points_in_bin = 1) const {
            VectorSet<float,3> ds_colors;
            getDownsampledColors(ds_colors, min_points_in_bin);
            return ds_colors;
        }

        const PointsNormalsColorsGridDownsampler& getDownsampledPointsNormalsColors(VectorSet<ScalarT,EigenDim> &ds_points,
                                                                                    VectorSet<ScalarT,EigenDim> &ds_normals,
                                                                                    VectorSet<float,3> &ds_colors,
                                                                                    size_t min_points_in_bin = 1) const
        {
            ds_points.resize(this->data_map_.rows(), this->grid_lookup_table_.size());
            ds_normals.resize(this->data_map_.rows(), this->grid_lookup_table_.size());
            ds_colors.resize(3, this->grid_lookup_table_.size());

            ScalarT scale;
            size_t ind = 0;
            for (size_t k = 0; k < this->bin_iterators_.size(); k++) {
                if (this->bin_iterators_[k]->second.pointCount < min_points_in_bin) {
                    continue;
                } else {
                    scale = (ScalarT)(1.0)/this->bin_iterators_[k]->second.pointCount;
                    ds_points.col(ind) = scale*this->bin_iterators_[k]->second.pointSum;
                    ds_normals.col(ind) = (scale*this->bin_iterators_[k]->second.normalSum).normalized();
                    ds_colors.col(ind++) = scale*this->bin_iterators_[k]->second.colorSum;
                }
            }

            ds_points.conservativeResize(Eigen::NoChange, ind);
            ds_normals.conservativeResize(Eigen::NoChange, ind);
            ds_colors.conservativeResize(Eigen::NoChange, ind);
            return *this;
        }
    };
}
