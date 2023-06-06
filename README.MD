Co bys m?l dod?lat
- p?idání materiál?, každý objekt by m?l obsahovat materiál (ambient_material, diffuse_material, specular_material;) který pak p?edáváš fragment shaderu aby um?l pracovat se sv?tlem
- každý objekt by m?l mít svojí velikost kv?li kolizím


Na?ítání model?
- OBJloader.cpp u m? v projektu 
    - loadOBJ je vykradená z jeho repa, a žere to cestu k soubor? a arraye na verticies, uvs(kv?li texturám, a normály)
    - Metoda createMesh vytvo?í meshObjekt (já to mám custom opravené ale vycházím z mesh_p_m)
    - mesh by m?l mít metodu Draw která vezme veritices, normály, uv?ka a vykresí to
    