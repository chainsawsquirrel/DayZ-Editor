
static FileDialogResult ExportExpansionData(EditorObjectDataMap data, string filename)
{
	FileHandle handle = OpenFile(filename, FileMode.WRITE);
	
	if (!handle) {
		return FileDialogResult.IN_USE;
	}
	
	foreach (EditorObjectData editor_object: data) {
		// Land_Construction_House2|13108.842773 10.015385 6931.083984|-101.999985 0.000000 0.000000
		vector position = editor_object.GetPosition();
		vector orientation = editor_object.GetOrientation();
		string line = string.Format("%1|%2 %3 %4|%5 %6 %7", editor_object.Type, position[0], position[1], position[2], orientation[0], orientation[1], orientation[2]);
		FPrintln(handle, line);
	}
	
	if (handle) {
		CloseFile(handle);
	}
	
	return FileDialogResult.SUCCESS;
}

class ExpansionImportData
{

	static void ReadFromFile(out EditorObjectDataMap data, string filename)
	{
		FileHandle handler = OpenFile(filename, FileMode.READ);
		
		string name;
		vector position, rotation;
		while (GetObjectFromFile(handler, name, position, rotation)) {	
					
			EditorObjectData dta = EditorObjectData.Create(name, position, rotation, EditorObjectFlags.ALL);
			Print(dta);
			data.InsertData(dta);
		}
		
		// another stupid fix
		EditorObjectMap placed_objects = GetEditor().GetPlacedObjects();		
		foreach (EditorObject eo: placed_objects) {
			GetEditor().SelectObject(eo);
		}
		GetEditor().ClearSelection();
		
		if (handler) {
			CloseFile(handler);
		}
	}
	
    private static bool GetObjectFromFile(FileHandle file, out string name, out vector position, out vector rotation, out string special = "false")
    {                
        string line;
        int lineSize = FGets( file, line );
        
        if ( lineSize < 1 )
            return false;
        
        ref TStringArray tokens = new TStringArray;
        line.Split( "|", tokens );

        name = tokens.Get( 0 );        
        position = tokens.Get( 1 ).ToVector();
        rotation = tokens.Get( 2 ).ToVector();    
        special = tokens.Get( 3 );

        return true;
    }
}