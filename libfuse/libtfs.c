//%cflags:  -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -lfuse -pthread
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "include/libtfs.h"

//TODO add file size calculation
//TODO add directories

//TODO rename this

//TODO make this take a path from command line
/**
* @brief Sets up the internal data
* @details This function should be called right at the beginning of main().
*/
void init_tfs()
{
    backing_storage_path = (char*)malloc(100);

    sprintf(backing_storage_path, "blargh");

    START("init_tfs");
    for (inode_t inode = 0; inode < NUM_FILES; inode++)
    {
        files[inode].used = false;
    }
    LOG1("initialized files");
    for (blockno_t block = 0; block < NUM_BLOCKS; block++)
    {
        blocks[block].allocated = false;
        blocks[block].next = -1;
    }
    LOG1("initialized blocks");

    backing_storage = fopen(backing_storage_path, "r+");

    int backing_storage_fd = fileno(backing_storage);
/*
    pread(backing_storage_fd, files, sizeof(files) * NUM_FILES, files_origin);
    pread(backing_storage_fd, blocks, sizeof(block) * NUM_BLOCKS, blocks_origin);
*/
    END("init_tfs");
    //sanity_check();
}

void restore()
{
  backing_storage_path = (char*)malloc(100);
  sprintf(backing_storage_path, "blargh");
  
  FILE *handle;
  int off = BLOCKSIZE*NUM_BLOCKS;
  handle = fopen("blargh", "r");
  fseek(handle,off,SEEK_SET);
  int i;
  for(i=0;i<NUM_FILES;i++)
  {
    fread(&files[i],sizeof(struct file),1,handle);
  }
  //LOG("RESTORE : %s",files[5].path);
  //LOG("RESTORE : %s",files[5].start_block);
  //LOG("RESTORE : %s",files[5].is_dir);
  //LOG("RESTORE : %s",files[5].used);
  for(i=0;i<NUM_BLOCKS;i++)
  {
    fread(&blocks[i],sizeof(struct block),1,handle);
  }
  fread(&file_count,sizeof(inode_t),1,handle);
  fclose(handle);
}

/**
* @brief Sync all the data to backing store
* @details Writes all the metadata(not thread-safe), and then calls fsync to update the data.
*/
void sync()
{
/*
    START("sync");
    files_origin = 0;
    blocks_origin = sizeof(file) * NUM_FILES;
    data_origin = sizeof(file) * NUM_FILES + sizeof(blocks) * NUM_BLOCKS;
    fseek(backing_storage, 0, 0);
    fwrite(files, sizeof(files), NUM_FILES, backing_storage);
    fseek(backing_storage, 0, blocks_origin);
    fwrite(blocks, sizeof(blocks), NUM_FILES, backing_storage);
    fsync(fileno(backing_storage));
    END("sync");
*/
    int off = BLOCKSIZE*NUM_BLOCKS;
    FILE *handle;
    handle = fopen("blargh", "r+");
    fseek(handle,off,SEEK_SET);
    int i;
    for(i=0;i<NUM_FILES;i++)
    {
      fwrite(&files[i],sizeof(struct file),1,handle);
    }
    for(i=0;i<NUM_BLOCKS;i++)
    {
      fwrite(&blocks[i],sizeof(struct block),1,handle);
    }
    fwrite(&file_count,sizeof(inode_t),1,handle);
    fclose(handle);
}
