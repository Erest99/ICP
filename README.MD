Co bys m?l dod?lat
- p?id�n� materi�l?, ka�d� objekt by m?l obsahovat materi�l (ambient_material, diffuse_material, specular_material;) kter� pak p?ed�v� fragment shaderu aby um?l pracovat se sv?tlem
- ka�d� objekt by m?l m�t svoj� velikost kv?li koliz�m


Na?�t�n� model?
- OBJloader.cpp u m? v projektu 
    - loadOBJ je vykraden� z jeho repa, a �ere to cestu k soubor? a arraye na verticies, uvs(kv?li textur�m, a norm�ly)
    - Metoda createMesh vytvo?� meshObjekt (j� to m�m custom opraven� ale vych�z�m z mesh_p_m)
    - mesh by m?l m�t metodu Draw kter� vezme veritices, norm�ly, uv?ka a vykres� to
    