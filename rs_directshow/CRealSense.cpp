#include "CRealSense.h"

void CRealSense::Initialize()
{
	auto ctx = rs2::context();
	auto devices = ctx.query_devices();
	frame_buffer.resize(image_width * image_height * 2 * 3);

	if (devices.size() == 0) { return; }

	// Use a configuration object to request only depth from the pipeline
	cfg.enable_stream(RS2_STREAM_DEPTH, image_width, image_height, RS2_FORMAT_Z16, frame_rate);
	cfg.enable_stream(RS2_STREAM_COLOR, image_width, image_height, RS2_FORMAT_BGR8, frame_rate);
	// Start streaming with the above configuration
	pipe.start(cfg);

	rs2::sensor ir_sensor;
	auto active_sensors = pipe.get_active_profile().get_device().query_sensors();
	for (auto target_sensor : active_sensors)
	{
		if (strcmp(target_sensor.get_info(rs2_camera_info::RS2_CAMERA_INFO_NAME), "Stereo Module") == 0) { ir_sensor = target_sensor; }
	}

	// stereo camera settings
	ir_sensor.set_option(rs2_option::RS2_OPTION_GAIN, gain);
	ir_sensor.set_option(rs2_option::RS2_OPTION_LASER_POWER, emitter_power);
	if (exposure_time > 0)
	{
		ir_sensor.set_option(rs2_option::RS2_OPTION_ENABLE_AUTO_EXPOSURE, 0);
		ir_sensor.set_option(rs2_option::RS2_OPTION_EXPOSURE, exposure_time);
	}
	else
	{
		ir_sensor.set_option(rs2_option::RS2_OPTION_ENABLE_AUTO_EXPOSURE, 1);
	}

	// filter settings
	thr_filter.set_option(RS2_OPTION_MIN_DISTANCE, min_depth);
	thr_filter.set_option(RS2_OPTION_MAX_DISTANCE, max_depth);
	depth_to_disparity = rs2::disparity_transform(true);
	spat_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE, 2.0f);
	spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.5f);
	spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 20.0f);
	temp_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.4f);

	// color filter setting
	color_filter.set_option(RS2_OPTION_HISTOGRAM_EQUALIZATION_ENABLED, 0);
	color_filter.set_option(RS2_OPTION_COLOR_SCHEME, 9.0f);
	color_filter.set_option(RS2_OPTION_MIN_DISTANCE, min_depth);
	color_filter.set_option(RS2_OPTION_MAX_DISTANCE, max_depth);

	is_ready = true;
}

unsigned char* CRealSense::Run()
{
	if (!is_ready) 
	{
		Initialize(); 
		return &(frame_buffer[0]);
	}

	rs2::frameset current_frame;
	pipe.poll_for_frames(&current_frame); // Wait for next set of frames from the camera
	if (!current_frame) { return NULL; } // Should not happen but if the pipeline is configured differently

	current_frame = align_to_color.process(current_frame);

	rs2::video_frame depth_frame = current_frame.get_depth_frame(); //Take the depth frame from the frameset

	// apply post processing filters. should be moded to another thread to reduce processing load
	depth_frame = thr_filter.process(depth_frame);
	depth_frame = depth_to_disparity.process(depth_frame);
	depth_frame = spat_filter.process(depth_frame);
	//		depth_frame = temp_filter.process(depth_frame);
	depth_frame = color_filter.process(depth_frame);

	rs2::video_frame color_frame = current_frame.get_color_frame(); //Take the color frame from the frameset

//	memcpy(&(frame_buffer[0]), color_frame.get_data(), sizeof(unsigned char) * image_width * image_height * 3);
//	memcpy(&(frame_buffer[image_width * image_height * 3]), depth_frame.get_data(), sizeof(unsigned char) * image_width * image_height * 3);

	// upside down flip and colorized depth color channel invert from RGB->BGR
	auto color_data = (unsigned char*)color_frame.get_data();
	for (int y = 0; y < image_height; y++)
	{
		memcpy(&frame_buffer[image_width * (image_height * 2 - y - 1) * 3], &color_data[image_width * y * 3], sizeof(unsigned char) * image_width * 3);
	}

	auto depth_data = (unsigned char*)depth_frame.get_data();
	for (int y = 0; y < image_height; y++)
	{
		for (int x = 0; x < image_width; x++)
		{
			frame_buffer[(image_width * (image_height - y - 1) + x) * 3		] = depth_data[(image_width * y + x) * 3 + 2];
			frame_buffer[(image_width * (image_height - y - 1) + x) * 3 + 1 ] = depth_data[(image_width * y + x) * 3 + 1];
			frame_buffer[(image_width * (image_height - y - 1) + x) * 3 + 2 ] = depth_data[(image_width * y + x) * 3];
		}
	}

	// upside down flip
/*
	std::vector<unsigned char> line_buffer;
	line_buffer.resize(sizeof(unsigned char) * image_width * 3);
	for (int y = 0; y < image_height; y++) 
	{
		memcpy(&(line_buffer[0]), &frame_buffer[image_width * y * 3], sizeof(unsigned char) * image_width * 3);
		memcpy(&frame_buffer[image_width * y * 3], &frame_buffer[image_width * (image_height * 2 - y - 1) * 3], sizeof(unsigned char) * image_width * 3);
		memcpy(&frame_buffer[image_width * (image_height * 2 - y - 1) * 3], &(line_buffer[0]), sizeof(unsigned char) * image_width * 3);
	}
*/
	return &(frame_buffer[0]);
}

void CRealSense::Finalize()
{
//	if(is_ready)
//		pipe.stop();
}