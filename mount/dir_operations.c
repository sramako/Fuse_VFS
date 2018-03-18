#include "include/libtfs.h"
#include "mount.h"


int do_readdir(char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    START("do_readdir");
    int i, j, count, root = 0;
    LOG("--> Getting The List of Files of %s\n", path);
    display();
    for (i = 0; i < file_count; i++)
    {
        root = 0, count = 0;
        for (j = 0; j < strlen(get_path(i)); j++)
        {
            LOG("Testing : %d\t%c|\n", get_path(i)[j] - '/', get_path(i)[j]);
            if (get_path(i)[j] == '/')
            {
                count++;
            }
        }
        if (count == 0)
        {
            root = 1;
        }
        LOG("\nROOT = %d\n", root);
        count = 0;
        for (j = 0; j < strlen(path) - 1; j++)
        {
            if (path[j + 1] - get_path(i)[j] != 0)
            {
                count++;
            }
        }
        if (count == 0)
        {
            if (strlen(get_path(i)) < strlen(path))
            {
                count += 100;
            }
        }
        int c1 = 0, c2 = 0;
        for (j = 0; j < strlen(path); j++)
        {
            if (path[j] - '/' == 0)
            {
                c1++;
            }
        }
        LOG("%s - %d\n", path, c1);
        for (j = 0; j < strlen(get_path(i)); j++)
        {
            if (get_path(i)[j] - '/' == 0)
            {
                c2++;
            }
        }
        if (get_path(i)[strlen(get_path(i)) - 1] - '/' != 0)
        {
            c2++;
        }
        if (path[strlen(path) - 1] - '/' == 0)
        {
            c1--;
        }
        LOG("%s - %d\n", get_path(i), c2);
        if (c2 - c1 > 1)
        {
            count += 1000;
        }

        if (files[i].used==false)
        {
            LOG("Check Path >> %s\n", path);
            count += 100000;
        }

        LOG("\nMismatch = %d\n_____________________\n", count);
        if (((strcmp(path, "/") == 0 && root) || count == 0) && (count < 100000))
        {
            for (j = strlen(get_path(i)) - 1; j >= 0; j--)
            {
                if (get_path(i)[j] - '/' == 0)
                {
                    break;
                }
            }
            LOG("Show > %s , %d\n", get_path(i), j);

            int s = j;
            char temp;
            if (s > 0)
            {
                char temp[strlen(get_path(i)) - j];
                for (j = 0; j < strlen(get_path(i)) - j - 1; j++)
                {
                    LOG("Buffering %c\n", strdup(get_path(j))[j + s + 1]);
                    temp[j] = strdup(get_path(i))[j + s + 1];
                }
                temp[j] = '\0';
                LOG("Individual entry : %s\n\n", temp);
                filler(buffer, strdup(temp), NULL, 0);
            }
            else
            {
                LOG("%s\n\n", get_path(i));
                filler(buffer, strdup(get_path(i)), NULL, 0);
            }
        }
        /*
        if(strlen(path)==1 && root)
        {
            filler( buffer, get_path(i), NULL, 0 );
        }
        */
    }
    LOG("RETURN 0\n");
    return 0;
}

int do_mkdir(char *path, mode_t mode)
{
    START("do_mkdir");
    mode = O_CREAT;
    LOG("Mkdir Called\n");
    files[file_count].is_dir = true;
    int i;
    char path2[PATH_LENGTH];
    for (i = 0; i < strlen(path) - 1; i++)
    {
        path2[i] = path[i + 1];
    }
    path2[i] = '\0';
    insert(strdup(path2), 1);
    char *cp;
    cp = strdup(path2);
    strcat(cp, "/.");
    //dir[file_count] = 2;
    insert(cp, 0);
    cp = strdup(path2);
    strcat(cp, "/..");
    files[file_count].is_dir = true;
    insert(cp, 0);
    return 0;
}

int do_unlink(char *path)
{
    START("do_unlink");
    LOG("Unlink called.\n");
    rmv(strdup(path));
    //LOG(">>%d\n", dir[getinode(path)]);
    LOG(">>%d\n", files[getinode(path)].is_dir);
    display();
    return 0;
}

int do_rmdir(char *path)
{
    START("do_unlink");
    LOG("Unlink called.\n");
    START("do_readdir");
    int i, j, count, root = 0,ffcount=0;
    LOG("--> Getting The List of Files of %s\n", path);
    display();
    for (i = 0; i < file_count; i++)
    {
        root = 0, count = 0;
        for (j = 0; j < strlen(get_path(i)); j++)
        {
            LOG("Testing : %d\t%c|\n", get_path(i)[j] - '/', get_path(i)[j]);
            if (get_path(i)[j] == '/')
            {
                count++;
            }
        }
        if (count == 0)
        {
            root = 1;
        }
        LOG("\nROOT = %d\n", root);
        count = 0;
        for (j = 0; j < strlen(path) - 1; j++)
        {
            if (path[j + 1] - get_path(i)[j] != 0)
            {
                count++;
            }
        }
        if (count == 0)
        {
            if (strlen(get_path(i)) < strlen(path))
            {
                count += 100;
            }
        }
        int c1 = 0, c2 = 0;
        for (j = 0; j < strlen(path); j++)
        {
            if (path[j] - '/' == 0)
            {
                c1++;
            }
        }
        LOG("%s - %d\n", path, c1);
        for (j = 0; j < strlen(get_path(i)); j++)
        {
            if (get_path(i)[j] - '/' == 0)
            {
                c2++;
            }
        }
        if (get_path(i)[strlen(get_path(i)) - 1] - '/' != 0)
        {
            c2++;
        }
        if (path[strlen(path) - 1] - '/' == 0)
        {
            c1--;
        }
        LOG("%s - %d\n", get_path(i), c2);
        if (c2 - c1 > 1)
        {
            count += 1000;
        }

        if (files[i].used==false)
        {
            LOG("Check Path >> %s\n", path);
            count += 100000;
        }

        LOG("\nMismatch = %d\n_____________________\n", count);
        if (((strcmp(path, "/") == 0 && root) || count == 0) && (count < 100000))
        {
            for (j = strlen(get_path(i)) - 1; j >= 0; j--)
            {
                if (get_path(i)[j] - '/' == 0)
                {
                    break;
                }
            }
            LOG("Show > %s , %d\n", get_path(i), j);

            int s = j;
            char temp;
            if (s > 0)
            {
                char temp[strlen(get_path(i)) - j];
                for (j = 0; j < strlen(get_path(i)) - j - 1; j++)
                {
                    LOG("Buffering %c\n", strdup(get_path(j))[j + s + 1]);
                    temp[j] = strdup(get_path(i))[j + s + 1];
                }
                temp[j] = '\0';
                LOG("Individual entry : %s\n\n", temp);
                ffcount++;
                //filler(buffer, strdup(temp), NULL, 0);
            }
            else
            {
                LOG("%s\n\n", get_path(i));
                ffcount++;
                //filler(buffer, strdup(get_path(i)), NULL, 0);
            }
        }
        /*
        if(strlen(path)==1 && root)
        {
            filler( buffer, get_path(i), NULL, 0 );
        }
        */
    }
    LOG("FFCOUNT:%d",ffcount);
    if(ffcount<3)
    {
      rmv(strdup(path));
      LOG(">>%d\n", files[getinode(path)].is_dir);
      display();
      return 0;
    }
    //LOG(">>%d\n", dir[getinode(path)]);
    display();
    return -ENOTEMPTY;
}
