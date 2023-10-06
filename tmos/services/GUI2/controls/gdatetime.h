/*
 * gtime_edit.h
 *
 *  Created on: May 3, 2023
 *      Author: bratkov
 */

#ifndef GDATETIME_EDIT_H_
#define GDATETIME_EDIT_H_

#include <stdgui.h>
#include <gedit.h>
#include <tmos_time.h>

#define TIME_POS_MAP_SIZE	6

enum datetime_move_e {
	datetime_move_right=0,
	datetime_move_repeat_right=0x10,
	datetime_move_left=1,
	datetime_move_repeat_left= 0x11,
	datetime_move_up=2,
	datetime_move_down=3,
	datetime_move_last
};

struct time_pos_map
{
	struct{
		struct{
		int8_t pos;
		int8_t type;
		}__attribute__((packed)) map[TIME_POS_MAP_SIZE];
		int16_t index;
		int16_t max_index;
	}__attribute__((packed));
	time_pos_map(): index(0), max_index(0)
	{
		memset(map, -1, sizeof(map));
	}

	void init_map(const char* format);

	char get_type();
	char get_pos();
	bool inc_index();
	bool dec_index();

	inline bool is_index_valid()
	{
		return (max_index > 0 && index < max_index);
	}
};

struct GDateTime: GEdit
{
	const char* time_format;
	time_t* time;
	time_pos_map* map;

	GDateTime(GId id_t, const RECT_T& rect_t, uint32_t flgs, time_t* t, const char* fmt):
		GEdit(id_t, rect_t, "", nullptr, flgs, ES_DEFAULT | ES_WANTRETURN),
		time_format(fmt),
		time(t), map(nullptr)
	{
		t->sprintf(txt, fmt);
		// checking the number of available buttons
#if ((KEY_DRV_COUNT * KEY_RD_COUNT) + KEY_FIX_COUNT) <= 6
		align |= ES_USE_VIRTUAL_KEYBOARD;
#endif
	};

	virtual ~GDateTime()
	{
		if(map)
		{
			delete map;
			map = nullptr;
		}
	}

	POINT_T get_border_size() const override
	{
		return POINT_T(1,1);
	}
protected:
	unsigned int process_key (GMessage& msg) override;
	unsigned int initialize (GMessage& msg) override;
	void show_cursor() override;

private:
	unsigned int move_pos(datetime_move_e move);
	unsigned int update_time(char ch);
	unsigned int inc_dec_time(bool inc);
	void increment_txt_pos();
};



#endif /* GDATETIME_EDIT_H_ */
