#include <gtk/gtk.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

MYSQL mysql;

enum 
{
	ID_CLIENT,
	NAME_CLIENT,
	PHONE_CLIENT,
	ADDRESS_CLIENT,
	N_COLUMNS
};
enum
{
	ID_JOB,
	DATE_JOB,
	PERCENT_JOB,
	PRICE_JOB,
	JOBS
};
	struct sadd_window {
		GtkWidget *window;
		GtkWidget *name_client;
		GtkWidget *entry_name_client;
		GtkWidget *address_client;
		GtkWidget *entry_address_client;
		GtkWidget *phone_client;
		GtkWidget *entry_phone_client;
		GtkWidget *calendar;
		GtkWidget *punkts[18];
		GtkWidget *total_price;
		GtkWidget *entry_total_price;
		GtkWidget *total_percent;
		GtkWidget *entry_total_percent;
		GtkWidget *button_add_item;
		gchar *id;
	};

//GtkTreeStore *store_id_clients;

void
add_job_item ( GtkButton *button, gpointer data )
{
	struct clients_data {
		char punkts[18];
		const char *total_price;
		const char *total_percent;
	};

	struct clients_data client_data;
	struct sadd_window *s = (struct sadd_window *) data;

	client_data.total_price = gtk_entry_get_text ( (GtkEntry *) s->entry_total_price );
	client_data.total_percent = gtk_entry_get_text ( (GtkEntry *) s->entry_total_percent );
	for ( int i = 0; i < 18; i++ ) {
		client_data.punkts[i] = ( gtk_toggle_button_get_active ( (GtkToggleButton *) s->punkts[i] ) ) ? 1 : 0 ;
	}

	MYSQL_RES *res;
	MYSQL_ROW row;

	char *query = malloc ( 1024 );
	memset ( query, 0, 1024 );

	unsigned long *lengths;
	unsigned int i = 0;

			sprintf ( query,
				"insert into jobs ("
				"id,percent,total,"
				"punkt1,punkt2,punkt3,punkt4,punkt5,punkt6,punkt7,punkt8,punkt9,"
				"punkt10,punkt11,punkt12,punkt13,punkt14,punkt15,punkt16,punkt17,punkt18) "
				"VALUE ( '%s','%s','%s',"
				"%s,%s,%s,%s,%s,%s,%s,%s,%s,"
				"%s,%s,%s,%s,%s,%s,%s,%s,%s"
				");",
				s->id,
				client_data.total_percent,
				client_data.total_price,
				( client_data.punkts[0] ) ? "true" : "false",
				( client_data.punkts[1] ) ? "true" : "false",
				( client_data.punkts[2] ) ? "true" : "false",
				( client_data.punkts[3] ) ? "true" : "false",
				( client_data.punkts[4] ) ? "true" : "false",
				( client_data.punkts[5] ) ? "true" : "false",
				( client_data.punkts[6] ) ? "true" : "false",
				( client_data.punkts[7] ) ? "true" : "false",
				( client_data.punkts[8] ) ? "true" : "false",
				( client_data.punkts[9] ) ? "true" : "false",
				( client_data.punkts[10] ) ? "true" : "false",
				( client_data.punkts[11] ) ? "true" : "false",
				( client_data.punkts[12] ) ? "true" : "false",
				( client_data.punkts[13] ) ? "true" : "false",
				( client_data.punkts[14] ) ? "true" : "false",
				( client_data.punkts[15] ) ? "true" : "false",
				( client_data.punkts[16] ) ? "true" : "false",
				( client_data.punkts[17] ) ? "true" : "false"
					);
				mysql_query ( &mysql, query );

				gtk_widget_hide ( (GtkWidget *) s->button_add_item );
#if 0
		if (s->button_add_item ) gtk_widget_destroy ( (GtkWidget *) s->button_add_item );
		if (s->entry_total_percent ) gtk_widget_destroy ( (GtkWidget *) s->entry_total_percent );
		if (s->total_percent ) gtk_widget_destroy ( (GtkWidget *) s->total_percent );
		if (s->entry_total_price) gtk_widget_destroy ( (GtkWidget *) s->entry_total_price );
		if (s->total_price) gtk_widget_destroy ( (GtkWidget *) s->total_price );
		for ( int i = 0; i < 18; i++ ) {
			gtk_widget_destroy ( s->punkts [ i ] );
		}
		if (s->window) gtk_widget_destroy ( (GtkWidget *) s->window );
#endif
		free ( s );
		free ( query );

}
void
add_job ( GtkButton *button, gpointer data )
{
	struct clients_data {
		const char *name;
		const char *address;
		const char *phone;
		char punkts[18];
		const char *total_price;
		const char *total_percent;
	};
	struct clients_data client_data;
	struct sadd_window *s = (struct sadd_window *) data;

	client_data.total_price = gtk_entry_get_text ( (GtkEntry *) s->entry_total_price );
	client_data.total_percent = gtk_entry_get_text ( (GtkEntry *) s->entry_total_percent );
	client_data.name = gtk_entry_get_text ( (GtkEntry *)s->entry_name_client );
	client_data.address = gtk_entry_get_text ( (GtkEntry *) s->entry_address_client );
	client_data.phone = gtk_entry_get_text ( (GtkEntry *) s->entry_phone_client );
	for ( int i = 0; i < 18; i++ ) {
		client_data.punkts[i] = ( gtk_toggle_button_get_active ( (GtkToggleButton *) s->punkts[i] ) ) ? 1 : 0 ;
	}

	char *query = malloc ( 1024 );
	memset ( query, 0, 1024 );
	sprintf ( query,
			"insert into clients "
			"(name,phone,address) "
			"VALUE ('%s','%s','%s');",
			client_data.name,
			client_data.phone,
			client_data.address );


	mysql_query ( &mysql, query );
	MYSQL_RES *res;
	MYSQL_ROW row;

	res = mysql_store_result ( &mysql );

	memset ( query, 0, 1024 );

	sprintf ( query,
			"select id from clients where "
			"name = '%s' and phone = '%s' and address = '%s';"
			, 
			client_data.name,
			client_data.phone,
			client_data.address );

	mysql_query ( &mysql, query );


	unsigned long *lengths;
	unsigned int i = 0;

	res = mysql_store_result ( &mysql );

	row = mysql_fetch_row ( res );
	if ( row ) {
		lengths = mysql_fetch_lengths ( res );
			char *id = malloc ( lengths[i] + 1 );
			memset ( id, 0, lengths[i] + 1 );
			sprintf ( id, "%.*s", lengths[i], row[i] );
			sprintf ( query,
				"insert into jobs ("
				"id,percent,total,"
				"punkt1,punkt2,punkt3,punkt4,punkt5,punkt6,punkt7,punkt8,punkt9,"
				"punkt10,punkt11,punkt12,punkt13,punkt14,punkt15,punkt16,punkt17,punkt18) "
				"VALUE ( '%s','%s','%s',"
				"%s,%s,%s,%s,%s,%s,%s,%s,%s,"
				"%s,%s,%s,%s,%s,%s,%s,%s,%s"
				");",
				id,
				client_data.total_percent,
				client_data.total_price,
				( client_data.punkts[0] ) ? "true" : "false",
				( client_data.punkts[1] ) ? "true" : "false",
				( client_data.punkts[2] ) ? "true" : "false",
				( client_data.punkts[3] ) ? "true" : "false",
				( client_data.punkts[4] ) ? "true" : "false",
				( client_data.punkts[5] ) ? "true" : "false",
				( client_data.punkts[6] ) ? "true" : "false",
				( client_data.punkts[7] ) ? "true" : "false",
				( client_data.punkts[8] ) ? "true" : "false",
				( client_data.punkts[9] ) ? "true" : "false",
				( client_data.punkts[10] ) ? "true" : "false",
				( client_data.punkts[11] ) ? "true" : "false",
				( client_data.punkts[12] ) ? "true" : "false",
				( client_data.punkts[13] ) ? "true" : "false",
				( client_data.punkts[14] ) ? "true" : "false",
				( client_data.punkts[15] ) ? "true" : "false",
				( client_data.punkts[16] ) ? "true" : "false",
				( client_data.punkts[17] ) ? "true" : "false"
					);
				mysql_query ( &mysql, query );
				free ( id );
	}

		mysql_free_result ( res );
		gtk_widget_hide ( (GtkWidget *) s->button_add_item );

#if 0
		gtk_widget_destroy ( (GtkWidget *) s->button_add_item );
		gtk_widget_destroy ( (GtkWidget *) s->entry_total_percent );
		gtk_widget_destroy ( (GtkWidget *) s->total_percent );
		gtk_widget_destroy ( (GtkWidget *) s->entry_total_price );
		gtk_widget_destroy ( (GtkWidget *) s->total_price );
		gtk_widget_destroy ( (GtkWidget *) s->name_client );
		gtk_widget_destroy ( (GtkWidget *) s->entry_name_client );
		gtk_widget_destroy ( (GtkWidget *) s->address_client );
		gtk_widget_destroy ( (GtkWidget *) s->entry_address_client );
		gtk_widget_destroy ( (GtkWidget *) s->phone_client );
		gtk_widget_destroy ( (GtkWidget *) s->entry_phone_client );
		for ( int i = 0; i < 18; i++ ) {
			gtk_widget_destroy ( s->punkts [ i ] );
		}
		gtk_widget_destroy ( (GtkWidget *) s->window );
#endif
		free ( s );
		free ( query );

}

void
add_job_of_clients ( GtkToolButton * toolbutton, gpointer user_data )
{
	gchar *id = ( gchar *) user_data;
	struct sadd_window *sw = malloc ( sizeof ( struct sadd_window ) );
	sw->id = id;
	sw->window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_modal ( (GtkWindow *) sw->window, TRUE );
	gtk_window_set_default_size ( (GtkWindow *) sw->window, 300, 200 );
	sw->punkts[0] = gtk_check_button_new_with_label ( "Установка Windows 10/8/7/Vista/XP" );
	sw->punkts[1] =  gtk_check_button_new_with_label ( "Установка драйверов" );
	sw->punkts[2] =  gtk_check_button_new_with_label ( "Установка Microsoft Office 2010/2007/2003" );
	sw->punkts[3] =  gtk_check_button_new_with_label ( "Установка антивируса" );
	sw->punkts[4] =  gtk_check_button_new_with_label ( "Поиск и удаление вирусов" );
	sw->punkts[5] =  gtk_check_button_new_with_label ( "Удаление вредоносного баннера" );
	sw->punkts[6] =  gtk_check_button_new_with_label ( "Настройка проводного интернета" );
	sw->punkts[7] =  gtk_check_button_new_with_label ( "Настройка WIFI роутера" );
	sw->punkts[8] =  gtk_check_button_new_with_label ( "Замена жесткого диска" );
	sw->punkts[9] = gtk_check_button_new_with_label ( "Замена CD/DVD привода" );
	sw->punkts[10] = gtk_check_button_new_with_label ( "Замена материнской платы" );
	sw->punkts[11] = gtk_check_button_new_with_label ( "Замена внешних разъёмов системной платы" );
	sw->punkts[12] = gtk_check_button_new_with_label ( "Замена корпусных деталей" );
	sw->punkts[13] = gtk_check_button_new_with_label ( "Замена оперативной памяти" );
	sw->punkts[14] = gtk_check_button_new_with_label ( "Замена центрального процессора" );
	sw->punkts[15] = gtk_check_button_new_with_label ( "Замена/Ремонт системы охлаждения" );
	sw->punkts[16] = gtk_check_button_new_with_label ( "Работа по замене клавиатуры" );
	sw->punkts[17] = gtk_check_button_new_with_label ( "Работа по замене TFT панели (матрицы) ноутбука" );

	GtkWidget *grid = gtk_grid_new ( );

	for ( int i = 0, y = 0, x = 0; i < 18; i++, y++ ) {
		if ( i == 9 ) { x = 1; y = 0; }
		gtk_grid_attach ( (GtkGrid *) grid,
				(GtkWidget *) sw->punkts[i],
				x,y,
				1,1);
	}


	sw->total_price = gtk_label_new ( "Оплата" );
	sw->entry_total_price = gtk_entry_new ( );
	gtk_entry_set_width_chars ( (GtkEntry *) sw->entry_total_price, 40 );
	gtk_entry_set_max_length ( (GtkEntry *) sw->entry_total_price, 39 );
	sw->total_percent = gtk_label_new ( "Процент от сделки" );
	sw->entry_total_percent = gtk_entry_new ( );
	gtk_entry_set_width_chars ( (GtkEntry *) sw->entry_total_percent, 40 );
	gtk_entry_set_max_length ( (GtkEntry *) sw->entry_total_percent, 39 );
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->total_price,
			0,9,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->entry_total_price,
			1,9,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->total_percent,
			0,10,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->entry_total_percent,
			1,10,
			1,1);

	sw->button_add_item = gtk_button_new_with_label ( "Добавить запись" );
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->button_add_item,
			0, 15,
			2,1);

	g_signal_connect ( (GtkWidget *) sw->button_add_item, "clicked", G_CALLBACK(add_job_item), (gpointer)sw);
	gtk_container_add ( (GtkContainer *) sw->window, (GtkWidget *) grid );
	gtk_widget_show_all ( sw->window );
}
void
add_window ( GtkToolButton * toolbutton, gpointer user_data )
{
	struct sadd_window *sw = malloc ( sizeof ( struct sadd_window ) );
	sw->window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_modal ( (GtkWindow *) sw->window, TRUE );
	gtk_window_set_default_size ( (GtkWindow *) sw->window, 300, 200 );
	sw->name_client = gtk_label_new ( "Имя клиента" );
	sw->entry_name_client = gtk_entry_new ( );
	gtk_entry_set_width_chars ( (GtkEntry *) sw->entry_name_client, 40 );
	gtk_entry_set_max_length ( (GtkEntry *) sw->entry_name_client, 128 );
	sw->address_client = gtk_label_new ( "Адрес клиента" );
	sw->entry_address_client = gtk_entry_new ( );
	gtk_entry_set_width_chars ( (GtkEntry *) sw->entry_address_client, 40 );
	gtk_entry_set_max_length ( (GtkEntry *) sw->entry_address_client, 128 );
	sw->phone_client = gtk_label_new ( "Номер телефона" );
	sw->entry_phone_client = gtk_entry_new ( );
	gtk_entry_set_width_chars ( (GtkEntry *) sw->entry_phone_client, 40 );
	gtk_entry_set_max_length ( (GtkEntry *) sw->entry_phone_client, 39 );
	sw->punkts[0] = gtk_check_button_new_with_label ( "Установка Windows 10/8/7/Vista/XP" );
	sw->punkts[1] =  gtk_check_button_new_with_label ( "Установка драйверов" );
	sw->punkts[2] =  gtk_check_button_new_with_label ( "Установка Microsoft Office 2010/2007/2003" );
	sw->punkts[3] =  gtk_check_button_new_with_label ( "Установка антивируса" );
	sw->punkts[4] =  gtk_check_button_new_with_label ( "Поиск и удаление вирусов" );
	sw->punkts[5] =  gtk_check_button_new_with_label ( "Удаление вредоносного баннера" );
	sw->punkts[6] =  gtk_check_button_new_with_label ( "Настройка проводного интернета" );
	sw->punkts[7] =  gtk_check_button_new_with_label ( "Настройка WIFI роутера" );
	sw->punkts[8] =  gtk_check_button_new_with_label ( "Замена жесткого диска" );
	sw->punkts[9] = gtk_check_button_new_with_label ( "Замена CD/DVD привода" );
	sw->punkts[10] = gtk_check_button_new_with_label ( "Замена материнской платы" );
	sw->punkts[11] = gtk_check_button_new_with_label ( "Замена внешних разъёмов системной платы" );
	sw->punkts[12] = gtk_check_button_new_with_label ( "Замена корпусных деталей" );
	sw->punkts[13] = gtk_check_button_new_with_label ( "Замена оперативной памяти" );
	sw->punkts[14] = gtk_check_button_new_with_label ( "Замена центрального процессора" );
	sw->punkts[15] = gtk_check_button_new_with_label ( "Замена/Ремонт системы охлаждения" );
	sw->punkts[16] = gtk_check_button_new_with_label ( "Работа по замене клавиатуры" );
	sw->punkts[17] = gtk_check_button_new_with_label ( "Работа по замене TFT панели (матрицы) ноутбука" );

	GtkWidget *grid = gtk_grid_new ( );
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->name_client,
			0,0,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->entry_name_client,
			1,0,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->address_client,
			0,1,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->entry_address_client,
			1,1,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->phone_client,
			0,2,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->entry_phone_client,
			1,2,
			1,1);

	for ( int i = 0, y = 3, x = 0; i < 18; i++, y++ ) {
		if ( i == 9 ) { x = 1; y = 3; }
		gtk_grid_attach ( (GtkGrid *) grid,
				(GtkWidget *) sw->punkts[i],
				x,y,
				1,1);
	}


	sw->total_price = gtk_label_new ( "Оплата" );
	sw->entry_total_price = gtk_entry_new ( );
	gtk_entry_set_width_chars ( (GtkEntry *) sw->entry_total_price, 40 );
	gtk_entry_set_max_length ( (GtkEntry *) sw->entry_total_price, 39 );
	sw->total_percent = gtk_label_new ( "Процент от сделки" );
	sw->entry_total_percent = gtk_entry_new ( );
	gtk_entry_set_width_chars ( (GtkEntry *) sw->entry_total_percent, 40 );
	gtk_entry_set_max_length ( (GtkEntry *) sw->entry_total_percent, 39 );
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->total_price,
			0,12,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->entry_total_price,
			1,12,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->total_percent,
			0,13,
			1,1);
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->entry_total_percent,
			1,13,
			1,1);

	sw->button_add_item = gtk_button_new_with_label ( "Добавить запись" );
	gtk_grid_attach ( (GtkGrid *) grid,
			(GtkWidget *) sw->button_add_item,
			0, 15,
			2,1);

	g_signal_connect ( (GtkWidget *) sw->button_add_item, "clicked", G_CALLBACK(add_job), (gpointer)sw);
	gtk_container_add ( (GtkContainer *) sw->window, (GtkWidget *) grid );
	gtk_widget_show_all ( sw->window );
}
void
add_tables ( GtkToolButton * toolbutton, gpointer user_data )
{
	mysql_query ( &mysql,
			"create table clients ("
			"id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
			"name CHAR(128),"
			"phone CHAR(39),"
			"address CHAR(128)"
			");"
			);
	mysql_query ( &mysql,
			"create table jobs ("
			"number INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
			"id INT,"
			"date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
			"punkt1 BOOL,"
			"punkt2 BOOL,"
			"punkt3 BOOL,"
			"punkt4 BOOL,"
			"punkt5 BOOL,"
			"punkt6 BOOL,"
			"punkt7 BOOL,"
			"punkt8 BOOL,"
			"punkt9 BOOL,"
			"punkt10 BOOL,"
			"punkt11 BOOL,"
			"punkt12 BOOL,"
			"punkt13 BOOL,"
			"punkt14 BOOL,"
			"punkt15 BOOL,"
			"punkt16 BOOL,"
			"punkt17 BOOL,"
			"punkt18 BOOL,"
			"percent FLOAT,"
			"total FLOAT"
			");"
			);
}
struct get_job_info {
	GtkWidget *client_id;
	GtkTreeStore *store_id_clients;
	gchar *id;
};
void
get_job_items ( GtkToolButton *toolbutton, gpointer data )
{
	struct get_job_info *gji = (struct get_job_info *) data;
	gtk_tree_store_clear ( (GtkTreeStore *) gji->store_id_clients );
	MYSQL_RES *res;
	MYSQL_ROW row;

	char *query = malloc ( 1024 );
	memset ( query, 0, 1024 );
	sprintf ( query,
			"select number, date, percent, total from jobs where id = '%s';",
			gji->id
			);

	mysql_query ( &mysql, query );
	free ( query );


	unsigned long *lengths;
	unsigned int i = 0;

	res = mysql_store_result ( &mysql );

	GtkTreeIter iter_id;

	while ( row = mysql_fetch_row ( res ) ) {
		gtk_tree_store_append ( gji->store_id_clients, &iter_id, NULL );
		gtk_tree_store_set ( gji->store_id_clients, &iter_id,
			ID_JOB, row[0],
			DATE_JOB, row[1],
			PERCENT_JOB, row[2],
			PRICE_JOB, row[3],
			-1 );
	}
	mysql_free_result ( res );
}
struct info_job {
	GtkWidget *window;
	GtkWidget *punkts[18];
};
void
activate_job ( GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column,
		gpointer data  )
{
	GtkTreeSelection *select;
	select = gtk_tree_view_get_selection ( (GtkTreeView *) tree_view );
	gtk_tree_selection_set_mode ( select, GTK_SELECTION_SINGLE );
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *id;
	if ( gtk_tree_selection_get_selected ( select, &model, &iter ) ) {
		gtk_tree_model_get ( model, &iter, ID_JOB, &id, -1 );

		struct info_job * ij = malloc ( sizeof ( struct info_job ) );

		ij->window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
		GtkWidget *grid = gtk_grid_new ( );

		ij->punkts[0] = gtk_check_button_new_with_label ( "Установка Windows 10/8/7/Vista/XP" );
		ij->punkts[1] =  gtk_check_button_new_with_label ( "Установка драйверов" );
		ij->punkts[2] =  gtk_check_button_new_with_label ( "Установка Microsoft Office 2010/2007/2003" );
		ij->punkts[3] =  gtk_check_button_new_with_label ( "Установка антивируса" );
		ij->punkts[4] =  gtk_check_button_new_with_label ( "Поиск и удаление вирусов" );
		ij->punkts[5] =  gtk_check_button_new_with_label ( "Удаление вредоносного баннера" );
		ij->punkts[6] =  gtk_check_button_new_with_label ( "Настройка проводного интернета" );
		ij->punkts[7] =  gtk_check_button_new_with_label ( "Настройка WIFI роутера" );
		ij->punkts[8] =  gtk_check_button_new_with_label ( "Замена жесткого диска" );
		ij->punkts[9] = gtk_check_button_new_with_label ( "Замена CD/DVD привода" );
		ij->punkts[10] = gtk_check_button_new_with_label ( "Замена материнской платы" );
		ij->punkts[11] = gtk_check_button_new_with_label ( "Замена внешних разъёмов системной платы" );
		ij->punkts[12] = gtk_check_button_new_with_label ( "Замена корпусных деталей" );
		ij->punkts[13] = gtk_check_button_new_with_label ( "Замена оперативной памяти" );
		ij->punkts[14] = gtk_check_button_new_with_label ( "Замена центрального процессора" );
		ij->punkts[15] = gtk_check_button_new_with_label ( "Замена/Ремонт системы охлаждения" );
		ij->punkts[16] = gtk_check_button_new_with_label ( "Работа по замене клавиатуры" );
		ij->punkts[17] = gtk_check_button_new_with_label ( "Работа по замене TFT панели (матрицы) ноутбука" );
		for ( int i = 0, y = 0, x = 0; i < 18; i++, y++ ) {
			if ( i == 9 ) { x = 1; y = 0; }
			gtk_grid_attach ( (GtkGrid *) grid,
					(GtkWidget *) ij->punkts[i],
					x,y,
					1,1);
		}
		char *query = malloc ( 1024 );
		memset ( query, 0, 1024 );
		sprintf ( query,
			"select punkt1, punkt2, punkt3, punkt4, punkt5, punkt6, punkt7, punkt8, punkt9 "
			"punkt10, punkt11, punkt12, punkt13, punkt14, punkt15, punkt16, punkt17, punkt18 "
			"from jobs where number = '%s';",
				id
				);

		mysql_query ( &mysql, query );
		free ( query );

		MYSQL_ROW row;
		MYSQL_RES *res;

		unsigned long *lengths;
		unsigned int i = 0;

		res = mysql_store_result ( &mysql );

		GtkTreeIter iter_id;

		row = mysql_fetch_row ( res );
		for ( int i = 0; i < 18; i++ ) {
			if ( row[i][0] == '1' ) {
				gtk_toggle_button_set_active ( (GtkToggleButton *) ij->punkts[i], TRUE );
			}
		}

		gtk_container_add ( (GtkContainer *) ij->window, grid );
		gtk_widget_show_all ( (GtkWidget *) ij->window );
		
	}
}

void
activate_client ( GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column,
		gpointer data  )
{
	struct get_job_info *gji = malloc ( sizeof ( struct get_job_info ) );
	struct sadd_window *sw = malloc ( sizeof ( struct sadd_window ) );
	GtkTreeSelection *select;
	select = gtk_tree_view_get_selection ( (GtkTreeView *) tree_view );
	gtk_tree_selection_set_mode ( select, GTK_SELECTION_SINGLE );
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *id;
	if ( gtk_tree_selection_get_selected ( select, &model, &iter ) ) {
		gtk_tree_model_get ( model, &iter, ID_CLIENT, &id, -1 );
		sw->id = id;
		gji->id = id;

		GtkWidget *window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
		gtk_window_set_title ( (GtkWindow *) window, "Информация о клиенте" );
		gtk_window_set_default_size ( (GtkWindow *) window, 400, 400 );
		GtkWidget *main_box = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 0 );

		GtkWidget *toolbar;
		toolbar = gtk_toolbar_new ( );

		gtk_toolbar_set_style ( (GtkToolbar *) toolbar, GTK_TOOLBAR_ICONS );
		GtkToolItem *toolbar_item_add = gtk_tool_button_new ( NULL, "add_item");
		gtk_tool_button_set_icon_name ( (GtkToolButton *) toolbar_item_add, "list-add-symbolic.symbolic" );
		gtk_toolbar_insert ( (GtkToolbar *) toolbar, (GtkToolItem *) toolbar_item_add, -1 );

		GtkToolItem *toolbar_item_get = gtk_tool_button_new ( NULL, "get_items");
		gtk_tool_button_set_icon_name ( (GtkToolButton *) toolbar_item_get, "go-down-symbolic.symbolic" );
		gtk_toolbar_insert ( (GtkToolbar *) toolbar, (GtkToolItem *) toolbar_item_get, -1 );

		gtk_box_pack_start ( (GtkBox *) main_box,
			(GtkWidget *) toolbar,
			FALSE,
			FALSE,
			0);

		GtkWidget *grid = gtk_grid_new ( );
		GtkWidget *label_name_client = gtk_label_new ( "Имя клиента" );
		sw->entry_name_client = gtk_entry_new ( );
		gtk_entry_set_width_chars ( (GtkEntry *) sw->entry_name_client, 40 );
		gtk_grid_attach ( (GtkGrid *) grid,
				(GtkWidget *) label_name_client,
				0, 0,
				1, 1);
		gtk_grid_attach ( (GtkGrid *) grid,
				(GtkWidget *) sw->entry_name_client,
				1, 0,
				1, 1);
		gtk_box_pack_start ( (GtkBox *) main_box,
				(GtkWidget *) grid,
				FALSE,
				FALSE,
				0);

		GtkWidget *label_phone_client = gtk_label_new ( "Номер клиента" );
		sw->entry_phone_client = gtk_entry_new ( );
		gtk_entry_set_width_chars ( (GtkEntry *) sw->entry_phone_client, 40 );
		gtk_grid_attach ( (GtkGrid *) grid,
				(GtkWidget *) label_phone_client,
				0, 1,
				1, 1);
		gtk_grid_attach ( (GtkGrid *) grid,
				(GtkWidget *) sw->entry_phone_client,
				1, 1,
				1, 1);
		GtkWidget *label_address_client = gtk_label_new ( "Адрес клиента" );
		sw->entry_address_client = gtk_entry_new ( );
		gtk_entry_set_width_chars ( (GtkEntry *) sw->entry_address_client, 40 );
		gtk_grid_attach ( (GtkGrid *) grid,
				(GtkWidget *) label_address_client,
				0, 2,
				1, 1);
		gtk_grid_attach ( (GtkGrid *) grid,
				(GtkWidget *) sw->entry_address_client,
				1, 2,
				1, 1);

		MYSQL_RES *res;
		MYSQL_ROW row;

		char *query = malloc ( 1024 );
		memset ( query, 0, 1024 );
		sprintf ( query,
				"select name, phone, address from clients where id='%s';",
				id);

		mysql_query ( &mysql, query );
		free ( query );

		res = mysql_store_result ( &mysql );

		unsigned long *lengths;
		unsigned int i = 0;

		row = mysql_fetch_row ( res );
		if ( row ) {
			lengths = mysql_fetch_lengths ( res );
			gtk_entry_set_text ( (GtkEntry *) sw->entry_name_client, row[0] );
			gtk_entry_set_text ( (GtkEntry *) sw->entry_phone_client, row[1] );
			gtk_entry_set_text ( (GtkEntry *) sw->entry_address_client, row[2] );

		}
		mysql_free_result ( res );

		g_signal_connect ( (GtkWidget *) toolbar_item_add, "clicked", G_CALLBACK (add_job_of_clients), id );

		GtkWidget *frame = gtk_frame_new ( "Выполненные работы" );
		gji->client_id = gtk_tree_view_new ( );
		GtkWidget *scroll = gtk_scrolled_window_new ( NULL, NULL );
		gtk_container_add ( (GtkContainer *) scroll, gji->client_id );
		gtk_container_add ( (GtkContainer *) frame, scroll );

		gtk_box_pack_end ( (GtkBox *) main_box,
				(GtkWidget *) frame,
				TRUE,
				TRUE,
				0);

		gji->store_id_clients = gtk_tree_store_new ( 4,
			G_TYPE_STRING,
			G_TYPE_STRING,
			G_TYPE_STRING,
			G_TYPE_STRING
			);

		GtkTreeViewColumn *column;
		GtkCellRenderer *renderer;
	
		gtk_tree_view_set_model ( (GtkTreeView *) gji->client_id, (GtkTreeModel *) gji->store_id_clients );
//		g_object_unref ( gji->store_id_clients );

		char *titles[] = {
			"id",
			"дата",
			"процент",
			"оплата",
			NULL
		};
		for ( int i = 0; titles[i] != NULL; i++ ) {
			column = gtk_tree_view_column_new ( );
			renderer = gtk_cell_renderer_text_new ( );
			gtk_tree_view_column_pack_end ( column, renderer, TRUE );
			gtk_tree_view_column_set_title ( column, titles[i] );
			gtk_tree_view_column_add_attribute (column, renderer, "text",i);
			gtk_tree_view_insert_column ( (GtkTreeView *) gji->client_id, column, -1 );
			gtk_tree_view_column_set_resizable ( column, TRUE );
		}


		g_signal_connect ( (GtkWidget *) toolbar_item_get, "clicked", G_CALLBACK (get_job_items), gji );
		g_signal_connect ( ( GtkWidget *) gji->client_id, "row-activated", G_CALLBACK (activate_job), NULL);
		gtk_container_add ( (GtkContainer *) window, main_box );
		gtk_widget_show_all ( (GtkWidget *) window );
	}
}

void
get_clients ( GtkToolButton *toolbutton, gpointer data )
{
	GtkTreeStore *store_id_clients = (GtkTreeStore *) data;
	gtk_tree_store_clear ( (GtkTreeStore *) store_id_clients );
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_query ( &mysql, "select * from clients;" );

	unsigned long *lengths;
	unsigned int i = 0;

	res = mysql_store_result ( &mysql );

	GtkTreeIter iter_id;


	while ( row = mysql_fetch_row ( res ) ) {
		lengths = mysql_fetch_lengths ( res );
		int total_length = mysql_num_fields ( res );
		gtk_tree_store_append ( store_id_clients, &iter_id, NULL );
		gtk_tree_store_set ( store_id_clients, &iter_id,
			ID_CLIENT, row[0],
			NAME_CLIENT, row[1],
			PHONE_CLIENT, row[2],
			ADDRESS_CLIENT, row[3],
			-1 );
	}
}

void
delete_event ( GtkApplication *app, GtkWindow *window, gpointer user_data )
{
	mysql_close ( &mysql );
}
struct search_item {
	GtkWidget *clients_id;
	GtkTreeStore *store_id_clients;
};
void
search_item ( GtkSearchEntry *entry, gpointer data )
{
	struct search_item *si = (struct search_item *) data;
	const gchar *search_str = gtk_entry_get_text ( (GtkEntry *) entry );
	int length_of_search = strlen ( search_str );
	if ( !length_of_search ) {
		gtk_tree_store_clear ( (GtkTreeStore *) si->store_id_clients );
		MYSQL_ROW row;
		MYSQL_RES *res;
		mysql_query ( &mysql, "select * from clients;" );

		unsigned long *lengths;
		unsigned int i = 0;

		res = mysql_store_result ( &mysql );

		GtkTreeIter iter_id;

		while ( row = mysql_fetch_row ( res ) ) {
			lengths = mysql_fetch_lengths ( res );
			int total_length = mysql_num_fields ( res );
			gtk_tree_store_append ( si->store_id_clients, &iter_id, NULL );
			gtk_tree_store_set ( si->store_id_clients, &iter_id,
				ID_CLIENT, row[0],
				NAME_CLIENT, row[1],
				PHONE_CLIENT, row[2],
				ADDRESS_CLIENT, row[3],
				-1 );
		}
	} else {
		gtk_tree_store_clear ( (GtkTreeStore *) si->store_id_clients );
		MYSQL_ROW row;
		MYSQL_RES *res;
		mysql_query ( &mysql, "select * from clients;" );

		unsigned long *lengths;
		unsigned int i = 0;

		res = mysql_store_result ( &mysql );

		GtkTreeIter iter_id;

		while ( row = mysql_fetch_row ( res ) ) {
			lengths = mysql_fetch_lengths ( res );
			int total_length = mysql_num_fields ( res );
			if ( g_strrstr ( row[1],search_str ) || 
					g_strrstr ( row[2],search_str ) || 
					g_strrstr ( row[3],search_str )) {
				gtk_tree_store_append ( si->store_id_clients, &iter_id, NULL );
				gtk_tree_store_set ( si->store_id_clients, &iter_id,
					ID_CLIENT, row[0],
					NAME_CLIENT, row[1],
					PHONE_CLIENT, row[2],
					ADDRESS_CLIENT, row[3],
					-1 );
			}
		}
	}
}
struct many_info {
	GtkWidget *window;
	GtkWidget *from_calendar;
	GtkWidget *to_calendar;
	GtkWidget *entry_my;
	GtkWidget *entry_company;
	GtkWidget *entry_total;
	GtkWidget *button_get;
};

void
show_many ( GtkButton *button, gpointer data )
{
	struct date_c {
		guint year;
		guint month;
		guint day;
	};
	struct date_c from, to;
	struct many_info *mi = (struct many_info *) data;

	gtk_calendar_get_date ( (GtkCalendar *) mi->from_calendar, &from.year, &from.month, &from.day );
	gtk_calendar_get_date ( (GtkCalendar *) mi->to_calendar, &to.year, &to.month, &to.day );

	MYSQL_ROW row;
	MYSQL_RES *res;

	char *query = malloc ( 1024 );
	memset ( query, 0, 1024 );

	sprintf ( query,
			"select percent, total from jobs where "
			"date between timestamp('%d-%d-%d 00:00:00') and timestamp('%d-%d-%d 23:59:59');",
			from.year, from.month + 1, from.day,
			to.year, to.month + 1, to.day );

	mysql_query ( &mysql, query );
	free ( query );

	res = mysql_store_result ( &mysql );

	double total_my = 0;
	double total_company = 0;
	double total_total = 0;
	while ( row = mysql_fetch_row ( res ) ) {
		unsigned int percent_my;
		unsigned int percent_company;
		double my;
		double company;
		double total;
		percent_my = atoi ( row[0] );
		percent_company = 100 - percent_my;
		total = atof ( row[1] );
		my = total * percent_my / 100;
		company = total * percent_company / 100;
		total_my += my;
		total_company += company;
		total_total += total;
	}

	char *total_my_str = malloc ( 64 );
	memset ( total_my_str, 0, 64 );
	char *total_company_str = malloc ( 64 );
	memset ( total_company_str, 0, 64 );
	char *total_total_str = malloc ( 64 );
	memset ( total_total_str, 0, 64 );
	sprintf ( total_my_str, "%f", total_my );
	sprintf ( total_company_str, "%f", total_company );
	sprintf ( total_total_str, "%f", total_total );
	gtk_entry_set_text ( (GtkEntry *) mi->entry_my, total_my_str );
	gtk_entry_set_text ( (GtkEntry *) mi->entry_company, total_company_str );
	gtk_entry_set_text ( (GtkEntry *) mi->entry_total, total_total_str );
	free ( total_my_str );
	free ( total_company_str );
	free ( total_total_str );

	mysql_free_result ( res );
}

void
many ( GtkToolButton *toolbutton, gpointer data )
{
	struct many_info *mi = malloc ( sizeof ( struct many_info ) );
	mi->window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_title ( (GtkWindow *) mi->window, "Сведения о заработке" );
	GtkWidget *label_my = gtk_label_new ( "Я заработал" );
	GtkWidget *label_company = gtk_label_new ( "Компания заработала" );
	GtkWidget *label_total = gtk_label_new ( "Всего выплачено" );
	mi->from_calendar = gtk_calendar_new ( );
	mi->to_calendar = gtk_calendar_new ( );
	mi->entry_my = gtk_entry_new ( );
	mi->entry_company = gtk_entry_new ( );
	mi->entry_total = gtk_entry_new ( );

	GtkWidget *grid = gtk_grid_new ( );
	GtkWidget *box = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 0 );
	gtk_box_pack_start ( (GtkBox *) box, grid, FALSE, FALSE, 0 );
	gtk_grid_attach ( (GtkGrid *) grid,
			mi->from_calendar,
			0, 0,
			1, 1);
	gtk_grid_attach ( (GtkGrid *) grid,
			mi->to_calendar,
			1, 0,
			1, 1);
	gtk_grid_attach ( (GtkGrid *) grid,
			label_my,
			0, 1,
			1, 1);
	gtk_grid_attach ( (GtkGrid *) grid,
			mi->entry_my,
			1, 1,
			1, 1);
	gtk_grid_attach ( (GtkGrid *) grid,
			label_company,
			0, 2,
			1, 1);
	gtk_grid_attach ( (GtkGrid *) grid,
			mi->entry_company,
			1, 2,
			1, 1);
	gtk_grid_attach ( (GtkGrid *) grid,
			label_total,
			0, 3,
			1, 1);
	gtk_grid_attach ( (GtkGrid *) grid,
			mi->entry_total,
			1, 3,
			1, 1);

	mi->button_get = gtk_button_new_with_label ( "Получить данные" );
	g_signal_connect ( (GtkWidget *) mi->button_get, "clicked", G_CALLBACK(show_many), mi );
	gtk_box_pack_end ( (GtkBox *) box, mi->button_get, TRUE, TRUE, 0 );
	gtk_container_add ( (GtkContainer *) mi->window, box );
	gtk_widget_show_all ( (GtkWidget *) mi->window );
}

static void
activate (GtkApplication *app, gpointer user_data )
{
	GtkWidget *main_window;
	main_window = gtk_application_window_new ( app );
	gtk_window_set_title ( (GtkWindow *) main_window, "Компьютерщик. версия 0.1" );
	gtk_window_set_default_size ( (GtkWindow *) main_window, 1024, 550 );
	g_signal_connect ( (GtkWidget *) app, "window-removed", G_CALLBACK(delete_event), NULL );

	GtkWidget *main_box = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 0 );

	GtkWidget *toolbar;
	toolbar = gtk_toolbar_new ( );

	gtk_toolbar_set_style ( (GtkToolbar *) toolbar, GTK_TOOLBAR_BOTH );
	GtkToolItem *toolbar_item_add = gtk_tool_button_new ( NULL, "Добавить контакт");
	gtk_tool_button_set_icon_name ( (GtkToolButton *) toolbar_item_add, "contact-new-symbolic.symbolic" );
	gtk_toolbar_insert ( (GtkToolbar *) toolbar, (GtkToolItem *) toolbar_item_add, -1 );
	g_signal_connect ( (GtkWidget *) toolbar_item_add, "clicked", G_CALLBACK (add_window), NULL );

	GtkToolItem *toolbar_item_get_client = gtk_tool_button_new ( NULL, "Получить список клиентов" );
	gtk_tool_button_set_icon_name ( (GtkToolButton *) toolbar_item_get_client, "go-down-symbolic.symbolic" );
	gtk_toolbar_insert ( (GtkToolbar *) toolbar, (GtkToolItem *) toolbar_item_get_client, -1 );

	GtkToolItem *toolbar_show_many = gtk_tool_button_new ( NULL, "Заработок" );
	gtk_tool_button_set_icon_name ( (GtkToolButton *) toolbar_show_many, "face-laugh-symbolic.symbolic" );
	g_signal_connect ( (GtkWidget *) toolbar_show_many, "clicked", G_CALLBACK (many), NULL );
	gtk_toolbar_insert ( (GtkToolbar *) toolbar, (GtkToolItem *) toolbar_show_many, -1 );
	GtkToolItem *toolbar_item_db = gtk_tool_button_new ( NULL, "Создать таблицы");
	gtk_tool_button_set_icon_name ( (GtkToolButton *) toolbar_item_db, "face-smile-big-symbolic.symbolic" );
	gtk_toolbar_insert ( (GtkToolbar *) toolbar, (GtkToolItem *) toolbar_item_db, -1 );
	g_signal_connect ( (GtkWidget *) toolbar_item_db, "clicked", G_CALLBACK (add_tables), NULL );

	gtk_widget_set_name ( (GtkWidget *) toolbar_item_add, "item_add" );

	GtkWidget *scroll = gtk_scrolled_window_new ( NULL, NULL );
	GtkWidget *search = gtk_search_entry_new ( );
	GtkWidget *box = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	gtk_box_pack_start ( (GtkBox *) box,
			(GtkWidget *) toolbar,
			TRUE,
			TRUE,
			0);
	gtk_box_pack_start ( (GtkBox *) box,
			(GtkWidget *) search,
			FALSE,
			TRUE,
			0);
	gtk_box_pack_start ( (GtkBox *) main_box,
			(GtkWidget *) box,
			FALSE,
			FALSE,
			0);
	gtk_box_pack_end ( (GtkBox *) main_box,
			(GtkWidget *) scroll,
			TRUE,
			TRUE,
			0);
	struct search_item *si = malloc ( sizeof ( struct search_item ) );
	si->clients_id = gtk_tree_view_new ( );
	gtk_container_add ( (GtkContainer *) scroll, si->clients_id );
	si->store_id_clients = gtk_tree_store_new ( N_COLUMNS,
			G_TYPE_STRING,
			G_TYPE_STRING,
			G_TYPE_STRING,
			G_TYPE_STRING
			);

	g_signal_connect ( (GtkWidget *) toolbar_item_get_client, "clicked", G_CALLBACK (get_clients), 
			si->store_id_clients );
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	
	gtk_tree_view_set_model ( (GtkTreeView *) si->clients_id, (GtkTreeModel *) si->store_id_clients );

	char *titles[] = {
		"id",
		"имя клиента",
		"номер телефона",
		"адрес клиента",
		NULL
	};
	for ( int i = 0; titles[i] != NULL; i++ ) {
		column = gtk_tree_view_column_new ( );
		renderer = gtk_cell_renderer_text_new ( );
		gtk_tree_view_column_pack_end ( column, renderer, TRUE );
		gtk_tree_view_column_set_title ( column, titles[i] );
		gtk_tree_view_column_add_attribute (column, renderer, "text",i);
		gtk_tree_view_insert_column ( (GtkTreeView *) si->clients_id, column, -1 );
		gtk_tree_view_column_set_resizable ( column, TRUE );
	}


	g_signal_connect ( (GtkWidget *) search, "search-changed", G_CALLBACK (search_item), si );
	g_signal_connect ( ( GtkWidget *) si->clients_id, "row-activated", G_CALLBACK (activate_client), NULL);

	gtk_container_add ( (GtkContainer *) main_window, main_box );
	gtk_widget_show_all ( main_window );
}

void
init_mysql_db ( )
{
	mysql_init ( &mysql );
	if ( mysql_real_connect ( &mysql, "localhost",
			"programmer",
			"programmer",
			"programmer",
			0,
			NULL,
			0) == NULL ) {
		printf ( "%s\n", mysql_error ( &mysql ) );
		printf ( "сначала настройте базу данных\n" );
		exit ( EXIT_FAILURE );
			}


}
int
main ( int argc, char **argv )
{
	init_mysql_db ( );
	GtkApplication *application;
	int status;
	application = gtk_application_new ( "org.xverizex.programmer", G_APPLICATION_FLAGS_NONE );
	g_signal_connect ( application, "activate", G_CALLBACK (activate), NULL );
	status = g_application_run ( G_APPLICATION (application), argc, argv );
	g_object_unref (application);
}
