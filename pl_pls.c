/*
 * japlay pls playlist loader plugin
 * Copyright Janne Kulmala 2010
 */
#include "common.h"
#include "japlay.h"
#include "playlist.h"
#include "utils.h"
#include "plugin.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static bool pls_detect(const char *filename)
{
	const char *ext = file_ext(filename);
	return ext && !strcasecmp(ext, "pls");
}

static int pls_load(struct playlist *playlist, const char *filename)
{
	FILE *f = fopen(filename, "r");
	if (!f)
		return -1;

	char row[512];
	while (fgets(row, sizeof(row), f)) {
		char *value = strchr(row, '=');
		if (value == NULL)
			continue;
		*value = 0;
		value++;
		trim(row);
		if (!memcmp(row, "File", 4)) {
			trim(value);
			char *fname = build_filename(filename, value);
			if (fname) {
				struct playlist_entry *entry
					= add_file_playlist(playlist, fname);
				if (entry)
					put_entry(entry);
				free(fname);
			}
		}
	}
	fclose(f);
	return 0;
}

static struct playlist_plugin plugin_info = {
	.size = sizeof(struct playlist_plugin),
	.name = "pls playlist loader",
	.detect = pls_detect,
	.load = pls_load,
};

struct playlist_plugin *get_playlist_plugin()
{
	return &plugin_info;
}
