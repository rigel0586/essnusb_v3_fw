enum zpos_t {
    ZPOS_NONE=0,
    PLACE_NEXT,
    ZTOP_MATCH,
    ZCENTER_MATCH,
    ZBOTTOM_MATCH
};

struct chamberstructitem {
    char const *name;	   // material or logicalVolume name (NULL: vacuum)
    double z_size;	   // thickness of material (-1: use logicXXX)

    zpos_t z_align;
};

extern chamberstructitem chamber_def[];











