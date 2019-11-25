#ifdef __cplusplus
extern "C"{
#endif

typedef void (*frame_ready)(void* opaque, char* data, unsigned long bytes);
typedef struct {
	void* opaque;
	frame_ready module_read;
} module_callback;

int init_camera(int width, int height, double frequency);
void start_camera(module_callback* wrapper);
void stop_camera();

#ifdef __cplusplus
}
#endif
