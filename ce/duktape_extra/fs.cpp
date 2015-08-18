//
//  fs.c
//  ce
//
//  Created by kaixuan on 15-6-12.
//  Copyright (c) 2015年 kaixuan. All rights reserved.
//

#include "fs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform/CCFileUtils.h"

using namespace ce;

static int jsb_read(duk_context *J)
{
    const char *filename = duk_to_string(J, 0);
    unsigned char* pBuffer = NULL;
    unsigned long bufferSize = 0;
    pBuffer = CCFileUtils::getInstance()->getFileData(filename, "rb", &bufferSize);
    if (pBuffer)
    {
        duk_push_lstring(J, (char*)pBuffer, bufferSize);
        delete []pBuffer;
        pBuffer = NULL;
    }
    else
    {
        duk_push_null(J);
    }
    
    return 1;
}



static int jsb_readFromZip(duk_context* J)
{
    const char* zipFilename = duk_to_string(J, 0);
    const char* filename = duk_to_string(J, 1);
    unsigned char* pBuffer = NULL;
    unsigned long bufferSize = 0;
    pBuffer = ce::CCFileUtils::getInstance()->getFileDataFromZip(zipFilename, filename, &bufferSize);
    if (pBuffer)
    {
        duk_push_lstring(J, (char*)pBuffer, bufferSize);
        delete []pBuffer;
        pBuffer = NULL;
    }
    else
    {
        duk_push_null(J);
    }
    
    return 1;
}


/**
 js usage: io.write(content, storagePath/file.txt)
 */
static int jsb_write(duk_context* ctx)
{
    const char* filename = duk_to_string(ctx, 0);
    duk_size_t len = 0;
    const char* data = duk_to_lstring(ctx, 1, &len);
    bool result = CCFileUtils::getInstance()->writeToFile((unsigned char*)data, len, filename);
    duk_push_boolean(ctx, result);
    return 1;
}


static int jsb_writeablePath(duk_context* ctx)
{
    std::string result = CCFileUtils::getInstance()->getWritablePath();
    duk_push_string(ctx, result.c_str());
    return 1;
}




static duk_function_list_entry io_funcs[] = {
    { "read", jsb_read, 1 },
    { "write", jsb_write, 2 },
    { "readFromZip", jsb_readFromZip, 2 },
    { "writeablePath", jsb_writeablePath, 0 },
    { NULL, NULL, 0 }
};


int js_register_fs(duk_context* J)
{
    duk_push_global_object(J);
    duk_push_object(J);
    duk_put_function_list(J, -1, io_funcs);
    duk_put_prop_string(J, -2, "io");
    duk_pop(J);  /* pop global */
    return 1;
}