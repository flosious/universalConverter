/*
 *  universalConverter, a tool for converting table like text 
 *  files in to other formats, for Linux and Windows.
 *  Copyright (C) 2016  Florian Bärwolf
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 */



// c++ headers
#include <iostream>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <string>
#include <cstring>
#include <vector>
#include <locale.h>
#include <algorithm>

#include "globalvars.hpp"
#include "functions.hpp"

// #define DEBUG

using namespace std;

GtkBuilder *builder;

int main( int    argc, char **argv )
{ 
//     fclose(stdout); fclose(stderr);
    
    #ifdef DEBUG
    cout << "Program start" << endl;
    #endif
    GtkWidget  *window;
    GError     *error = NULL;
    
    
    setlocale(LC_ALL, "C"); 
    
    /* Init GTK+ */
    gtk_init( &argc, &argv );

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if( ! gtk_builder_add_from_file( builder, "windows.glade", &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
        return( 1 );
    }
    
    /* load config_params into global */
    init( argc, argv );
    
    /* Get main window pointer from UI */
    window = GTK_WIDGET( gtk_builder_get_object( builder, "mainwindow" ) );

    /* Connect signals */
    gtk_builder_connect_signals( builder, NULL );

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show( window );

    /* Start main loop */
    gtk_main();
    
    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );
    return( 0 );
}