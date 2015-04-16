/* Mihail Szabolcs, 2015 - Released into the public domain. */
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef __cplusplus
}
#endif

#ifndef LUA2EXE_FILE_SIZE
	#define LUA2EXE_FILE_SIZE 0
#endif

char *lua2exe_read(const char *filename)
{
	FILE *fp;
	char *str;
	size_t size, str_size;

	fp = fopen(filename, "rb");
	if(fp == NULL)
		return NULL;

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);

	if(size <= LUA2EXE_FILE_SIZE || size - LUA2EXE_FILE_SIZE <= 0)
	{
		fclose(fp);
		return NULL;
	}

	fseek(fp, LUA2EXE_FILE_SIZE, SEEK_SET);

	str_size = size - LUA2EXE_FILE_SIZE;

	str = malloc(str_size + 1);
	if(str == NULL)
	{
		fclose(fp);
		return NULL;
	}

	if(fread(str, str_size, 1, fp) < 1)
	{
		free(str);
		fclose(fp);
		return NULL;
	}

	str[str_size] = '\0';

	fclose(fp);
	return str;
}

int lua2exe_run(int argc, char *argv[], const char *str)
{
	int i, ret;
	lua_State *L;
	
	L = luaL_newstate();
	luaL_openlibs(L);

	lua_getglobal(L, "os");

	if(!lua_istable(L, -1))
	{
		lua_close(L);
		return EXIT_FAILURE;
	}

	lua_newtable(L);

	for(i=0; i<argc; i++)
	{
		lua_pushnumber(L, i + 1);
		lua_pushstring(L, argv[i]);
		lua_settable(L, -3);
	}

	lua_setfield(L, -2, "argv");

	ret = luaL_loadstring(L, str);

	if(ret == 0)
		ret = lua_pcall(L, 0, LUA_MULTRET, 0);

	if(ret != 0)
	{
		fprintf(stderr, "%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
		lua_close(L);
		return EXIT_FAILURE;
	}

	lua_close(L);
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	int ret;
	char *str;
	
	str = lua2exe_read(argv[0]);

	if(str == NULL)
		return EXIT_FAILURE;

	ret = lua2exe_run(argc, argv, str);

	free(str);
	return ret;
}
