/* 
 *	HT Editor
 *	textedit.h
 *
 *	Copyright (C) 2001, 2002 Sebastian Biallas (sb@web-productions.de)
 *	Copyright (C) 2001, 2002 Stefan Weyergraf (stefan@weyergraf.de)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __TEXTEDIT_H__
#define __TEXTEDIT_H__

#include "htobj.h"
#include "textfile.h"

#include "htformat.h"

#define TEXTEDITOPT_NULL			0
#define TEXTEDITOPT_INPUTTABS		1
#define TEXTEDITOPT_UNDO			2

struct text_viewer_pos {
	UINT line;
	UINT pofs;
};

union text_search_pos {
	FILEOFS offset;
};

class ht_text_editor;

#define ATOM_HT_UNDO_DATA_INSERT		MAGICD("UND\x01")
#define ATOM_HT_UNDO_DATA_OVERWRITE	MAGICD("UND\x02")
#define ATOM_HT_UNDO_DATA_DELETE		MAGICD("UND\x03")
#define ATOM_HT_UNDO_DATA_DELETE2		MAGICD("UND\x04")
#define ATOM_HT_UNDO_DATA_SPLIT_LINE	MAGICD("UND\x05")
#define ATOM_HT_UNDO_DATA_JOIN_LINE	MAGICD("UND\x06")
#define ATOM_HT_UNDO_DATA_INSERT_BLOCK	MAGICD("UND\x07")
#define ATOM_HT_UNDO_DATA_DELETE_BLOCK	MAGICD("UND\x08")

/*
 *	CLASS ht_undo_data
 */
class ht_undo_data: public ht_data {
public:
	ht_undo_data();
	virtual bool combine(ht_undo_data *ud);
	virtual UINT getsize()=0;
	virtual void gettext(char *text, UINT maxlen)=0;
	virtual void apply(ht_text_editor *te)=0;
	virtual void unapply(ht_text_editor *te, bool *goto_only)=0;
};

/*
 *	CLASS ht_undo_data_delete_string
 */
class ht_undo_data_delete_string: public ht_undo_data {
	text_viewer_pos apos;           /* cursor before */
	text_viewer_pos bpos;           /* cursor after */
	void *string;
	UINT len;
public:
	ht_undo_data_delete_string(text_viewer_pos *apos, text_viewer_pos *bpos, void *string, UINT len);
	~ht_undo_data_delete_string();
	virtual bool combine(ht_undo_data *ud);
	virtual UINT getsize();
	virtual void gettext(char *text, UINT maxlen);
	virtual OBJECT_ID object_id() const;
	virtual void apply(ht_text_editor *te);
	virtual void unapply(ht_text_editor *te, bool *goto_only);
};

/*
 *	CLASS ht_undo_data_delete_string2
 */
class ht_undo_data_delete_string2: public ht_undo_data {
	text_viewer_pos apos;           /* cursor before */
	text_viewer_pos bpos;           /* cursor after */
	void *string;
	UINT len;
public:
	ht_undo_data_delete_string2(text_viewer_pos *apos, text_viewer_pos *bpos, void *string, UINT len);
	~ht_undo_data_delete_string2();
	virtual bool combine(ht_undo_data *ud);
	virtual UINT getsize();
	virtual void gettext(char *text, UINT maxlen);
	virtual OBJECT_ID object_id() const;
	virtual void apply(ht_text_editor *te);
	virtual void unapply(ht_text_editor *te, bool *goto_only);
};

/*
 *	CLASS ht_undo_data_insert_string
 */
class ht_undo_data_insert_string: public ht_undo_data {
	text_viewer_pos apos;
	text_viewer_pos bpos;
	text_viewer_pos cpos;
	void *string;
	UINT len;
public:
	ht_undo_data_insert_string(text_viewer_pos *apos, text_viewer_pos *bpos, void *string, UINT len);
	~ht_undo_data_insert_string();
	virtual bool combine(ht_undo_data *ud);
	virtual UINT getsize();
	virtual void gettext(char *text, UINT maxlen);
	virtual OBJECT_ID object_id() const;
	virtual void apply(ht_text_editor *te);
	virtual void unapply(ht_text_editor *te, bool *goto_only);
};

/*
 *	CLASS ht_undo_data_overwrite_string
 */
class ht_undo_data_overwrite_string: public ht_undo_data {
	text_viewer_pos apos;
	text_viewer_pos bpos;
	text_viewer_pos cpos;
	void *string;
	UINT len;
	void *string2;
	UINT len2;
public:
	ht_undo_data_overwrite_string(text_viewer_pos *apos, text_viewer_pos *bpos, void *string, UINT len, void *string2, UINT len2);
	~ht_undo_data_overwrite_string();
	virtual bool combine(ht_undo_data *ud);
	virtual UINT getsize();
	virtual void gettext(char *text, UINT maxlen);
	virtual OBJECT_ID object_id() const;
	virtual void apply(ht_text_editor *te);
	virtual void unapply(ht_text_editor *te, bool *goto_only);
};


/*
 *	CLASS ht_undo_data_split_line
 */
class ht_undo_data_split_line: public ht_undo_data {
	text_viewer_pos apos;
	text_viewer_pos bpos;
	UINT indent;
public:
	ht_undo_data_split_line(text_viewer_pos *apos, text_viewer_pos *bpos, UINT Indent);
	~ht_undo_data_split_line();
	virtual UINT getsize();
	virtual void gettext(char *text, UINT maxlen);
	virtual OBJECT_ID object_id() const;
	virtual void apply(ht_text_editor *te);
	virtual void unapply(ht_text_editor *te, bool *goto_only);
};

/*
 *	CLASS ht_undo_data_join_line
 */
class ht_undo_data_join_line: public ht_undo_data {
	text_viewer_pos apos;
	text_viewer_pos bpos;     
	text_viewer_pos cpos;
public:
	ht_undo_data_join_line(text_viewer_pos *apos, text_viewer_pos *bpos);
	~ht_undo_data_join_line();
	virtual UINT getsize();
	virtual void gettext(char *text, UINT maxlen);
	virtual OBJECT_ID object_id() const;
	virtual void apply(ht_text_editor *te);
	virtual void unapply(ht_text_editor *te, bool *goto_only);
};

/*
 *	CLASS ht_undo_data_insert_block
 */

class ht_undo_data_insert_block: public ht_undo_data {
	text_viewer_pos apos;
	text_viewer_pos bpos;     
	text_viewer_pos cpos;
	text_viewer_pos sel_start;
	text_viewer_pos sel_end;
	void *block;
	UINT size;
public:
	ht_undo_data_insert_block(text_viewer_pos *apos, text_viewer_pos *bpos, void *block, UINT size);
	~ht_undo_data_insert_block();
	virtual UINT getsize();
	virtual void gettext(char *text, UINT maxlen);
	virtual OBJECT_ID object_id() const;
	virtual void apply(ht_text_editor *te);
	virtual void unapply(ht_text_editor *te, bool *goto_only);
};

/*
 *	CLASS ht_undo_data_delete_block
 */

class ht_undo_data_delete_block: public ht_undo_data {
	text_viewer_pos apos;
	text_viewer_pos bpos;     
	text_viewer_pos cpos;
	text_viewer_pos sel_start;
	text_viewer_pos sel_end;
	void *block;
	UINT size;
public:
	ht_undo_data_delete_block(text_viewer_pos *apos, text_viewer_pos *bpos, text_viewer_pos *Sel_start, text_viewer_pos *Sel_end);
	~ht_undo_data_delete_block();
	virtual UINT getsize();
	virtual void gettext(char *text, UINT maxlen);
	virtual OBJECT_ID object_id() const;
	virtual void apply(ht_text_editor *te);
	virtual void unapply(ht_text_editor *te, bool *goto_only);
};

/*
 *	CLASS ht_text_editor_undo
 */
 
class ht_text_editor_undo: public ht_clist {
public:
	UINT size, max_size;
	int clean_state;
	int current_position;
	bool goto_state;
public:
	void init(UINT max_undo_size);
	void insert_undo(ht_text_editor *te, ht_undo_data *undo);
	bool is_clean();
	bool	is_clean(int i);
	int get_current_position();
	void mark_clean();
	void undo(ht_text_editor *te, bool place_cursor_first);
	void redo(ht_text_editor *te);
};

/*
 *	CLASS ht_text_viewer
 */

#define cmd_text_viewer_goto				HT_COMMAND(601)
#define cmd_text_viewer_change_highlight     HT_COMMAND(602)

#define cmd_text_editor_undo				HT_COMMAND(620)
#define cmd_text_editor_redo				HT_COMMAND(621)
#define cmd_text_editor_protocol			HT_COMMAND(622)
#define cmd_text_editor_delete_line		HT_COMMAND(623)

class ht_text_viewer: public ht_view {
friend class ht_undo_data;
friend class ht_undo_data_delete_string;
friend class ht_undo_data_delete_string2;
friend class ht_undo_data_insert_string;
friend class ht_undo_data_overwrite_string;
friend class ht_undo_data_split_line;
friend class ht_undo_data_join_line;
friend class ht_undo_data_insert_block;
friend class ht_undo_data_delete_block;
protected:
	ht_textfile *textfile;
	bool own_textfile;

	ht_list *lexers;
	
	ht_syntax_lexer *lexer;
	bool own_lexer;

	UINT cursorx, cursory;
	text_viewer_pos sel_start, sel_end;

	UINT top_line;
	UINT xofs;

	bool selectcursor;
	bool selectmode;
	text_viewer_pos selectstart;

	char *EOL_string;
	char *EOF_string;
	bool show_EOL;
	bool show_EOF;
	bool highlight_wrap;
	UINT	tab_size;

	ht_search_request *last_search_request;
	FILEOFS last_search_end_ofs;

/* new */
			int buf_lprint0(int x, int y, int c, int l, char *text);
			UINT char_vsize(char c, UINT x);
			void clipboard_copy_cmd();
			bool continue_search();
	virtual	vcp  get_bgcolor();
			void make_pos_physical(text_viewer_pos *p);
			void normalize_selection();
			UINT physical_cursorx();
			void popup_change_highlight();
			void render_meta(UINT x, UINT y, text_viewer_pos *pos, vcp color);
			void render_str(int x, int y, vcp color, text_viewer_pos *pos, UINT len, char *str, bool multi);
			void render_str_color(vcp *color, text_viewer_pos *pos);
			ht_search_result *search(ht_search_request *request, text_search_pos *start, text_search_pos *end);
			bool show_search_result(ht_search_result *result);
public:
			void init(bounds *b, bool own_textfile, ht_textfile *textfile, ht_list *lexers);
	virtual	void done();
/* overwritten */
	virtual	void config_changed();
	virtual	void draw();
	virtual	void handlemsg(htmsg *msg);
	virtual	void resize(int rw, int rh);
/* new */
	virtual	char *func(UINT i, bool execute);
			UINT get_line_length(UINT line);
			UINT get_line_vlength(UINT line);
			UINT get_line_indent(UINT line);
			void get_selection(text_viewer_pos *start, text_viewer_pos *end);
			bool goto_line(UINT line);
	/* position indicator string */
	virtual	void get_pindicator_str(char *buf);
	/* scrollbar pos */
	virtual	bool get_hscrollbar_pos(int *pstart, int *psize);
	virtual	bool get_vscrollbar_pos(int *pstart, int *psize);
	/* cursor */
	virtual	cursor_mode get_cursor_mode();
	virtual	void get_cursor_pos(text_viewer_pos *cursor);
	/* conversions */
	virtual	bool pos_to_offset(text_viewer_pos *pos, FILEOFS *ofs);
	/**/
			ht_syntax_lexer *get_lexer();
			int ppos_str(char *buf, UINT bufsize, text_viewer_pos *ppos);
			void set_lexer(ht_syntax_lexer *l, bool own_l);
			void set_textfile(ht_textfile *t, bool own_t);
			ht_textfile *get_textfile();
/**/
			UINT cursor_up(UINT n);
			UINT cursor_down(UINT n);
			UINT cursor_left(UINT n);
			UINT cursor_right(UINT n);
			void cursor_home();
			void cursor_end();
			void cursor_vput(UINT vx);
			void cursor_pput(UINT px);
			void cursor_set(text_viewer_pos *pos);
			UINT scroll_up(UINT n);
			UINT scroll_down(UINT n);
			UINT scroll_left(UINT n);
			UINT scroll_right(UINT n);
			void select_add(text_viewer_pos *from, text_viewer_pos *to);
			void select_clear();
			void select_end();
			void select_set(text_viewer_pos *from, text_viewer_pos *to);
			void select_start();
};

/*
 *	CLASS ht_text_editor
 */

class ht_text_editor: public ht_text_viewer {
protected:
	UINT edit_options;
	ht_text_editor_undo *undo_list;
	bool	auto_indent;
	bool overwrite_mode;
	
/* new */
			void clipboard_cut_cmd();
			void clipboard_delete_cmd();
			void clipboard_paste_cmd();
	virtual	vcp  get_bgcolor();
			bool save();
public:
			void init(bounds *b, bool own_textfile, ht_textfile *textfile, ht_list *lexers, UINT edit_options);
	virtual	void done();
/* overwritten */
	virtual	void config_changed();
	virtual	char *func(UINT i, bool execute);
	virtual	void handlemsg(htmsg *msg);
	/* position indicator string */
	virtual	void get_pindicator_str(char *buf);
	/* cursor mode */
	virtual	cursor_mode get_cursor_mode();
/* new */
			bool concat_lines(UINT a);
			void delete_chars(UINT line, UINT ofs, UINT count);
			void delete_lines(UINT line, UINT count);
			void indent(UINT line, UINT start, UINT size);
			void insert_chars(UINT line, UINT ofs, void *chars, UINT len);
			void insert_lines(UINT line, UINT count);
			void split_line(UINT a, UINT pos);
			void unindent(UINT line, UINT start, UINT size);
	/* undo/redo */
			void textoperation_apply(ht_undo_data *ud);
			void redo();
			void show_protocol();
			void undo(bool place_cursor_first);
};

extern int text_viewer_pos_compare(text_viewer_pos *a, text_viewer_pos *b);

#endif /* __TEXTEDIT_H__ */

