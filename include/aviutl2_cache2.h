// https://github.com/oov/aviutl2_plugin_sdk_for_c
// The MIT License / Copyright (c) 2025 oov
#pragma once

// This file based on cache2.h from AviUtl ExEdit2 Plugin SDK
// LICENSE: The MIT License / Copyright (c) 2025 Kenkun

// Any plugin can export the following function to enable cache support:
// Note: In input plugins, the image-file cache acquisition function is deprecated.
//
// Cache initialization function
//   void InitializeCache(struct aviutl2_cache_handle *cache)
//   Called before InitializePlugin()

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct aviutl2_pixel_rgba;

/**
 * Cache reference base structure
 * Cache references remain valid while this structure is retained
 */
struct aviutl2_cache_reference {
  void (*func_release)(void *instance);
  void *cache_instance;
};

/**
 * Release a cache reference
 * In C, call this explicitly because the original C++ API uses a destructor
 * @param reference Cache reference to release
 */
static inline void aviutl2_cache_reference_release(struct aviutl2_cache_reference *reference) {
  if (reference && reference->func_release && reference->cache_instance) {
    reference->func_release(reference->cache_instance);
    reference->func_release = NULL;
    reference->cache_instance = NULL;
  }
}

/**
 * Image cache data structure
 */
struct aviutl2_cache_image {
  struct aviutl2_cache_reference reference;

  /**
   * Pointer to image cache data (NULL if acquisition failed)
   * Image data is PIXEL_RGBA
   */
  struct aviutl2_pixel_rgba *buffer;

  /**
   * Image cache size
   */
  int width, height;
};

/**
 * Check whether image cache data was acquired successfully
 * @param image Image cache data
 * @return false if the data is unavailable
 */
static inline bool aviutl2_cache_image_available(struct aviutl2_cache_image const *image) {
  return image && image->buffer;
}

/**
 * Release image cache data
 * @param image Image cache data
 */
static inline void aviutl2_cache_image_release(struct aviutl2_cache_image *image) {
  aviutl2_cache_reference_release(&image->reference);
}

/**
 * Audio cache data structure
 */
struct aviutl2_cache_audio {
  struct aviutl2_cache_reference reference;

  /**
   * Pointer to audio cache data (left channel) (NULL if acquisition failed)
   * Audio data is PCM(float) 32-bit
   */
  float *buffer0;

  /**
   * Pointer to audio cache data (right channel) (NULL if acquisition failed)
   * Audio data is PCM(float) 32-bit
   */
  float *buffer1;

  /**
   * Number of samples in the audio cache
   */
  int sample_num;

  /**
   * Number of channels in the audio cache (1 = mono / 2 = stereo)
   * Only buffer0 is available when the channel count is 1
   */
  int channel_num;
};

/**
 * Get an audio cache channel buffer by index
 * @param audio Audio cache data
 * @param index Channel index
 * @return Buffer pointer for the requested channel, or NULL if unavailable
 */
static inline float *aviutl2_cache_audio_get_buffer(struct aviutl2_cache_audio const *audio, size_t index) {
  if (!audio) {
    return NULL;
  }
  if (index == 0) {
    return audio->buffer0;
  }
  if (index == 1) {
    return audio->buffer1;
  }
  return NULL;
}

/**
 * Check whether audio cache data was acquired successfully
 * @param audio Audio cache data
 * @return false if the data is unavailable
 */
static inline bool aviutl2_cache_audio_available(struct aviutl2_cache_audio const *audio) {
  return audio && audio->buffer0;
}

/**
 * Release audio cache data
 * @param audio Audio cache data
 */
static inline void aviutl2_cache_audio_release(struct aviutl2_cache_audio *audio) {
  aviutl2_cache_reference_release(&audio->reference);
}

//--------------------------------

/**
 * Media file image cache data structure
 */
struct aviutl2_cache_file_image {
  struct aviutl2_cache_reference reference;

  /**
   * Pointer to image cache data (NULL if acquisition failed)
   * Image data format is one of INPUT_PIXEL_FORMAT values
   */
  void const *buffer;

  /**
   * Image cache size
   */
  int width, height;

  /**
   * Number of bytes per row in image cache data
   */
  int pitch;

  /**
   * Pixel format of image cache data
   * Values correspond to enum aviutl2_input_pixel_format in aviutl2_filter2.h
   */
  int format;
};

/**
 * Check whether media file image cache data was acquired successfully
 * @param image Media file image cache data
 * @return false if the data is unavailable
 */
static inline bool aviutl2_cache_file_image_available(struct aviutl2_cache_file_image const *image) {
  return image && image->buffer;
}

/**
 * Release media file image cache data
 * @param image Media file image cache data
 */
static inline void aviutl2_cache_file_image_release(struct aviutl2_cache_file_image *image) {
  aviutl2_cache_reference_release(&image->reference);
}

//--------------------------------

/**
 * Video information structure
 */
struct aviutl2_video_info {
  double total_time; /**< Total duration */
  int frame_num;     /**< Total number of frames */
  int track_num;     /**< Number of tracks */
  int width, height; /**< Resolution */
  int rate, scale;   /**< Frame rate */
};

/**
 * Audio information structure
 */
struct aviutl2_audio_info {
  double total_time;  /**< Total duration */
  int64_t sample_num; /**< Total number of samples */
  int track_num;      /**< Number of tracks */
  int rate;           /**< Sampling rate */
  int channel;        /**< Number of channels */
};

//--------------------------------

/**
 * Cache handle
 * Various cache data can be created in the application's shared cache area
 * Note: Unlike script cache buffers (cache:xxxx), these are allocated in main memory
 */
struct aviutl2_cache_handle {
  /**
   * Get image cache data
   * @param identifier Cache identifier pointer. Specify any static pointer (such as aviutl2_cache_handle or
   *                   aviutl2_filter_plugin_table)
   * @param name Cache identifier name. Any name can be assigned
   * @return Image cache data. Use aviutl2_cache_image_available() to check for acquisition failure
   */
  struct aviutl2_cache_image (*get_image_cache)(void *identifier, wchar_t const *name);

  /**
   * Create image cache data
   * @param identifier Cache identifier pointer. Specify any static pointer (such as aviutl2_cache_handle or
   *                   aviutl2_filter_plugin_table)
   * @param name Cache identifier name. Any name can be assigned
   * @param width Image width of the cache to create
   * @param height Image height of the cache to create
   * @return Image cache data. Image data can be written to the returned cache
   */
  struct aviutl2_cache_image (*create_image_cache)(void *identifier, wchar_t const *name, int width, int height);

  /**
   * Get audio cache data
   * @param identifier Cache identifier pointer. Specify any static pointer (such as aviutl2_cache_handle or
   *                   aviutl2_filter_plugin_table)
   * @param name Cache identifier name. Any name can be assigned
   * @return Audio cache data. Use aviutl2_cache_audio_available() to check for acquisition failure
   */
  struct aviutl2_cache_audio (*get_audio_cache)(void *identifier, wchar_t const *name);

  /**
   * Create audio cache data
   * @param identifier Cache identifier pointer. Specify any static pointer (such as aviutl2_cache_handle or
   *                   aviutl2_filter_plugin_table)
   * @param name Cache identifier name. Any name can be assigned
   * @param sample_num Number of samples in the audio cache to create
   * @param channel_num Number of channels in the audio cache to create (1 = mono / 2 = stereo)
   * @return Audio cache data. Audio data can be written to the returned cache
   */
  struct aviutl2_cache_audio (*create_audio_cache)(void *identifier,
                                                   wchar_t const *name,
                                                   int sample_num,
                                                   int channel_num);

  /**
   * Deprecated because this is replaced by the new function
   */
  struct aviutl2_cache_image (*deprecated_get_image_file_cache)(wchar_t const *file);

  /**
   * Get media file video information
   * @param file Path to media file
   * @param info Pointer to video information storage
   * @param info_size Size of video information storage. If different from VIDEO_INFO, only that size is retrieved
   * @return true if obtained
   */
  bool (*get_video_file_info)(wchar_t const *file, struct aviutl2_video_info *info, int info_size);

  /**
   * Get media file audio information
   * @param file Path to media file
   * @param info Pointer to audio information storage
   * @param info_size Size of audio information storage. If different from AUDIO_INFO, only that size is retrieved
   * @return true if obtained
   */
  bool (*get_audio_file_info)(wchar_t const *file, struct aviutl2_audio_info *info, int info_size);

  /**
   * Get image data from an image file through the cache
   * @param file Path to image file
   * @return Image cache data. Use aviutl2_cache_file_image_available() to check for acquisition failure
   */
  struct aviutl2_cache_file_image (*get_image_file_cache)(wchar_t const *file);

  /**
   * Get image data from a media file through the cache
   * @param file Path to media file
   * @param track Track number
   * @param frame Frame number to retrieve
   * @return Image cache data. Use aviutl2_cache_file_image_available() to check for acquisition failure
   */
  struct aviutl2_cache_file_image (*get_video_file_cache)(wchar_t const *file, int track, int frame);

  /**
   * Get image data from a media file through the cache
   * @param file Path to media file
   * @param track Video track number
   * @param time Time of frame to retrieve
   * @return Image cache data. Use aviutl2_cache_file_image_available() to check for acquisition failure
   */
  struct aviutl2_cache_file_image (*get_video_file_cache_by_time)(wchar_t const *file, int track, double time);

  /**
   * Get audio data from a media file through the cache
   * Audio format is PCM(float) 32-bit 2ch
   * @param file Path to media file
   * @param track Audio track number
   * @param sample_index Sample position to retrieve
   * @param sample_num Number of samples to retrieve
   * @param buffer0 Destination buffer pointer for left channel samples
   * @param buffer1 Destination buffer pointer for right channel samples
   * @return Number of samples actually retrieved
   */
  int (*get_audio_file_data)(wchar_t const *file,
                             int track,
                             int64_t sample_index,
                             int sample_num,
                             float *buffer0,
                             float *buffer1);
};
