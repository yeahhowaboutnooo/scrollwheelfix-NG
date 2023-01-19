#include "Hooks.h"
#include <xbyak/xbyak.h>

namespace logger = SKSE::log;

namespace Hooks
{
	void fail() {
		logger::error("failed to fix scroll wheel zoom during object animations!");
		return;
	}
	
	uint64_t injectionPoint, origCodeSize;
	void ScrollWheelFix()
	{
		//1.5.97 c11600 67242
		auto fCallsDefineScrollBehaviour = REL::RelocationID(67242, 68542).address();
		if (!fCallsDefineScrollBehaviour) {
			fail();
			return;
		}
		uintptr_t offset = 0;
		
		switch (REL::Module::GetRuntime()) {
			case REL::Module::Runtime::SE:
				offset = 0x3d8;
				break;
			case REL::Module::Runtime::AE:
				offset = 0x3c8;
				break;
			default:
				fail();
				return;
		}
		
		injectionPoint = fCallsDefineScrollBehaviour + offset;

		struct OrigCode : Xbyak::CodeGenerator
		{
			OrigCode()
			{
				mov(eax, qword[rax + 0x118]);
			}
		};
		OrigCode orig;
		orig.ready();

		//erase the original instruction with nops
		std::vector<std::uint8_t> nops(orig.getSize());
		std::fill(nops.begin(), nops.end(), (std::uint8_t)0x90);
		REL::safe_write<std::uint8_t>(injectionPoint, nops);


		origCodeSize = orig.getSize();
		struct DeleteBit6 : Xbyak::CodeGenerator
		{
			DeleteBit6()
			{
				mov(eax, qword[rax + 0x118]);
				and_(cl, 0xdf); //delete bit #6
				jmp(ptr[rip]);
				dq(injectionPoint + origCodeSize);
			}
		};
		DeleteBit6 db6;
		db6.ready();

		static SKSE::Trampoline injectedCode;
		auto injectedCodeSize = db6.getSize() + 32; //write_branch5 allocates too much mem
		injectedCode.create(injectedCodeSize);

		//writes:
		//jmp from injectionPoint -> end_InjectedCode
		//jmp from end_InjectedCode -> start_InjectedCode
		injectedCode.write_branch<5>(injectionPoint, 
		                             injectedCode.allocate(db6));

		logger::info("injected Code @ {0:x}", injectionPoint);
		logger::info("fixed scroll wheel zoom during object animations!");
		
	}

	void Install()
	{
		ScrollWheelFix();
	}
}
