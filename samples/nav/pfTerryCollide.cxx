// Set up intersection masks for collision detection using 'mask' as the
// intersection mask. Cache normals and such inside geosets if the geometry
// is static, but not if it is dynamic, i.e., its vertices change.
void pfTerryCollide::nodeSetup( pfNode *node, int mode, int mask )
{
    int fullmode;

    if (mode == COLLIDE_DYNAMIC)
	fullmode = PFTRAV_SELF|PFTRAV_DESCEND|PFTRAV_IS_UNCACHE;
    else
	fullmode = PFTRAV_SELF|PFTRAV_DESCEND|PFTRAV_IS_CACHE;
    node->setTravMask(PFTRAV_ISECT, mask, fullmode, PF_SET);
}
