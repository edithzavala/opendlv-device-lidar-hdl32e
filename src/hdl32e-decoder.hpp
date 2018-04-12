/*
 * opendlv-device-lidar-hdl32e decodes HDL32e data realized for OpenDLV.
 * Copyright (C) 2018  Christian Berger
 * Copyright (C) 2017  Hang Yin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HDL32E_DECODER
#define HDL32E_DECODER

#include "opendlv-standard-message-set.hpp"

#include <cstdint>
#include <array>
#include <sstream>
#include <vector>

class HDL32eDecoder {
   private:
    HDL32eDecoder(const HDL32eDecoder &) = delete;
    HDL32eDecoder(HDL32eDecoder &&)      = delete;
    HDL32eDecoder &operator=(const HDL32eDecoder &) = delete;
    HDL32eDecoder &operator=(HDL32eDecoder &&) = delete;

   public:
    HDL32eDecoder() noexcept;
    ~HDL32eDecoder() = default;

   public:
    /**
     * This method decodes the next packet from HDL32e.
     *
     * @return PointCloud messages.
     */
    std::vector<opendlv::proxy::PointCloudReading> decode(const std::string &data) noexcept;

   private:
    void setupCalibration() noexcept;
    void index32sensorIDs() noexcept;

   private:
    const uint32_t MAX_POINT_SIZE{70000}; // The maximum number of points per frame. 
    uint8_t m_distanceEncoding{1}; // 0: cm; 1: 2mm. For now, always 0 = cm.

    std::array<float, 32> m_verticalAngle{};
    std::array<uint8_t, 32> m_sensorOrderIndex{}; // Specify the sensor ID order for each 32 points with increasing vertical angle for PointCloudReading.
    std::array<uint16_t, 32> m_32SensorsNoIntensity{}; // Store the distance values of the current 32 sensors for PointCloudReading without intensities.

    float m_startAzimuth{0.0f};
    float m_currentAzimuth{0.0f};
    float m_previousAzimuth{0.0f};
    uint32_t m_pointIndexCPC{0}; // Current number of points of the current frame for compact point cloud.

    std::stringstream m_distanceStringStreamNoIntensityPart1{}; // Layer 0, 1, 4, 7..., i.e., in addition to Layer 0, every 3rd layer from Layer 1 and resulting in 12 layers, without intensity
    std::stringstream m_distanceStringStreamNoIntensityPart2{}; // Layer 2, 3, 6, 9..., i.e., in addition to Layer 2, every 3rd layer from Layer 3 and resulting in 11 layers, without intensity
    std::stringstream m_distanceStringStreamNoIntensityPart3{}; // Layer 5, 8, 11..., i.e., every 3rd layer from Layer 5 and resulting in 9 layers, without intensity
};

#endif
