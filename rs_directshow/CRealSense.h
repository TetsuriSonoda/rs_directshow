#pragma once

#include <librealsense2/rs.hpp>

class CRealSense
{
protected:
	// RealSense initialization
	int image_width = 848;
	int image_height = 480;
	int frame_rate = 30;

	float min_depth = 0.29f;
	float max_depth = 10.0f;

	float exposure_time = 0;
	float gain = 16;

	float emitter_power = 60.0f;

	rs2::pipeline pipe;
	rs2::config cfg;

	// Declare filters
	rs2::threshold_filter thr_filter;   // Threshold  - removes values outside recommended range
	rs2::disparity_transform depth_to_disparity = rs2::disparity_transform(true);
	rs2::spatial_filter spat_filter;    // Spatial    - edge-preserving spatial smoothing
	rs2::temporal_filter temp_filter;   // Temporal   - reduces temporal noise
//	rs2::disparity_transform disparity_to_depth(false);
	rs2::colorizer color_filter;
	rs2::align align_to_color = rs2::align(RS2_STREAM_COLOR);

	std::vector<unsigned char> frame_buffer;
	bool is_ready = false;

public:
	void Initialize();
	unsigned char* Run();
	void Finalize();
};
