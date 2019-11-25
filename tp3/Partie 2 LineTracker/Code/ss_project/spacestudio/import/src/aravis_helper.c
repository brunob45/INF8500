#ifdef __cplusplus
extern "C"{
#endif

#include "aravis_helper.h"
#include <arv.h>
#include <arvstr.h>

static module_callback* module_cb = NULL;
static ArvCamera* camera = NULL;
static ArvStream* stream = NULL;
static int local_width = 0;
static int local_height = 0;
static int frame = 0;

static void new_buffer_cb(ArvStream* stream, void* null_data) {
	ArvBuffer* buffer;

	buffer = arv_stream_try_pop_buffer(stream);
	if (buffer != NULL) {
		if (arv_buffer_get_status (buffer) == ARV_BUFFER_STATUS_SUCCESS) {

			int width, height, buffer_size;

			/* get the buffer along with its size (bytes) */
			char* buffer_data = (char*)arv_buffer_get_data(buffer, &buffer_size);

			/* from the buffer, determine the width and height */
			arv_buffer_get_image_region(buffer, NULL, NULL, &width, &height);

			/* call spacestudio module */
			module_cb->module_read(module_cb->opaque, buffer_data, buffer_size);

			/* free buffer */
			arv_stream_push_buffer(stream, buffer);
		}
	}
}

int init_camera(int width, int height, double frequency) {
	arv_g_thread_init(NULL);
	arv_g_type_init();
	
	camera = arv_camera_new(NULL);
	if (camera == NULL)
		return 1;

	local_width = width;
	local_height = height;
	arv_camera_set_region(camera, 0, 0, width, height);
	stream = arv_camera_create_stream(camera, NULL, NULL);
	if (stream == NULL) {
		g_object_unref(camera);
		return 1;
	}
	
	unsigned int packet_timeout = 20;
	unsigned int frame_retention = 100;
	gint payload = arv_camera_get_payload(camera);
	g_object_set(stream, "packet-timeout", (unsigned) packet_timeout * 1000, "frame-retention", (unsigned) frame_retention * 1000, NULL);
		
	int i;
	for (i=0; i<50; i++)
		arv_stream_push_buffer(stream, arv_buffer_new(payload, NULL));

	arv_camera_set_acquisition_mode(camera, ARV_ACQUISITION_MODE_CONTINUOUS);
	arv_camera_set_frame_rate(camera, frequency);
	
	return 0;
}

void start_camera(module_callback* wrapper) {
	module_cb = wrapper;
	arv_camera_start_acquisition(camera);
	g_signal_connect(stream, "new-buffer", G_CALLBACK(new_buffer_cb), NULL);
	arv_stream_set_emit_signals (stream, TRUE);
}

void stop_camera() {
	arv_camera_stop_acquisition(camera);
	g_object_unref(stream);
	g_object_unref(camera);
}

#ifdef __cplusplus
}
#endif
