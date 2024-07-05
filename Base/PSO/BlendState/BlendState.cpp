#include "BlendState.h"

D3D12_BLEND_DESC BlendState::GetBlendDesc() const { return blendDesc; }

void BlendState::Setting() {

	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
}