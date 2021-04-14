#include <Flexcommander.h>
#include <stdio.h>
#include <stdbool.h>
#include <List.h>
#include <memory.h>

bool isRoot = true;

int ProbeDevices(FlexCommanderProbeInfo* info) {
    int status = blkid_probe_all(info->blkCache);

    if (status < 0){
        fprintf(stderr, "Can't probe devices!\n");
        return -1;
    }
    return 0;
}

PathListNode* IterateDevices(FlexCommanderProbeInfo* info) {
    blkid_dev device;
    blkid_dev_iterate iterator = blkid_dev_iterate_begin(info->blkCache);
    PathListNode* list = NULL;
    PathListNode node;

    while (blkid_dev_next(iterator, &device) == 0) {
        const char * devName = blkid_dev_devname(device);
        node.token = devName;
        PathListAdd(&list, node);

        if (isRoot) {
            blkid_probe probe = blkid_new_probe_from_filename(devName);
            if (probe == NULL) {
                node.token = "Launch util as root to get more info!\n";
                PathListAdd(&list, node);
                isRoot = false;
            }
            else {
                node.token = "\t";
                PathListAdd(&list, node);
                blkid_do_probe(probe);
                const char *fsType;
                blkid_probe_lookup_value(probe, "TYPE", &fsType, NULL);
                node.token = fsType;
                PathListAdd(&list, node);
            }
        }
        node.token = "\n";
        PathListAdd(&list, node);
    }
    blkid_dev_iterate_end(iterator);
    return list;
}