#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <json_object.h>
#include <json_tokener.h>

char input[99999];

int main(int argc, char **argv)
{
	ssize_t n = read(0, input, sizeof input - 1);
	if (n < 0)
		abort();
	struct json_object *o = json_tokener_parse((char*)input);
	puts(json_object_to_json_string_ext(o, JSON_C_TO_STRING_PRETTY));
}
