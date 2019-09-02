#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Int_Input.H>

class PointLabelWindow: public Fl_Window
{
public:
	PointLabelWindow(int w, int h, const char* title);
	~PointLabelWindow(void);
	Fl_Input * InputId;
	Fl_Round_Button *RB_GCP;
	Fl_Round_Button *RB_Tie;
	Fl_Round_Button *RB_TieLine;
	Fl_Round_Button *RB_TieLineHorizontal;
	Fl_Round_Button *RB_TieLineVertical;
	Fl_Round_Button *RB_ACSPoint;
	Fl_Input * InputLineId;
	Fl_Button * ButtonOk;
	Fl_Button * ButtonCancel;
	Fl_Check_Button *ChBInc;
	Fl_Int_Input *InputInc;
	bool IsGCP;

	void * win; //Pointer to parent window
private:
	//Wartosci jakie mialy poszczegolne RB przed wczytaniem punktu wiazacego:
	int	RB_Tie_val;
	int	RB_TieLine_val;
	int	RB_TieLineHorizontal_val;
	int	RB_TieLineVertical_val;
	
	int handle (int e);

	static void cb_Button_Cancel(Fl_Widget *w, void *v);
	inline void cb_Button_Cancel_i();
	
	static void cb_Button_Ok(Fl_Widget *w, void *v);
	inline void cb_Button_Ok_i();

	static void cb_RB_GCP(Fl_Widget *w, void *v);
	inline void cb_RB_GCP_i();

	static void cb_RB_ACSPoint(Fl_Widget *w, void *v);
	inline void cb_RB_ACSPoint_i();

	static void cb_RB_Tie(Fl_Widget *w, void *v);
	inline void cb_RB_Tie_i();

	static void cb_RB_TieLine(Fl_Widget *w, void *v);
	inline void cb_RB_TieLine_i();

	static void cb_RB_TieLineHorizontal(Fl_Widget *w, void *v);
	inline void cb_RB_TieLineHorizontal_i();

	static void cb_RB_TieLineVertical(Fl_Widget *w, void *v);
	inline void cb_RB_TieLineVertical_i();

	static void cb_InputId(Fl_Widget *w, void *v);
	inline void cb_InputId_i();
};
