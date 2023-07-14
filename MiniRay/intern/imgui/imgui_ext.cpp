#include "imgui_ext.h"

void DrawRowsBackground(int row_count, float line_height, float x1, float x2, float y_offset, ImU32 col_even, ImU32 col_odd)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	float y0 = ImGui::GetCursorScreenPos().y + (float)(int)y_offset;

	int row_display_start;
	int row_display_end;
	ImGui::CalcListClipping(row_count, line_height, &row_display_start, &row_display_end);
	for (int row_n = row_display_start; row_n < row_display_end; row_n++)
	{
		ImU32 col = (row_n & 1) ? col_odd : col_even;
		if ((col & IM_COL32_A_MASK) == 0)
			continue;
		float y1 = y0 + (line_height * row_n);
		float y2 = y1 + line_height;
		draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col);
	}
}