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

	void AllowPOVchangeDuringAnimObjectAnims()
	{
		auto PlayerControls__IsCamSwitchControlsEnabled_1407051A0 = REL::RelocationID(41263, 42342).address();
		if (!PlayerControls__IsCamSwitchControlsEnabled_1407051A0) {
			fail();
			return;
		}
		uintptr_t offset = 0x13;

		injectionPoint = PlayerControls__IsCamSwitchControlsEnabled_1407051A0 + offset;

		//replace "and AL, 1" with "or AL, 1"
		REL::safe_fill(injectionPoint, 0x0c, 1);

		logger::info("allowing POV change during Animations with AnimObjects!");
	}

	uint64_t PlayerCharacter__SwitchSkeleton_1406A1820;
	void fixAnimObjectsNotBeingParsed()
	{
		PlayerCharacter__SwitchSkeleton_1406A1820 = REL::RelocationID(39401, 40476).address();
		if (!PlayerCharacter__SwitchSkeleton_1406A1820)
		{
			fail();
			return;
		}
		uintptr_t offset = 0x7;

		struct SetRDXtoZero : Xbyak::CodeGenerator
		{
			SetRDXtoZero()
			{
				and_(dl,0);
				movzx(ebp,dl);
				mov(rsi,rcx);
				jmp(ptr[rip]);
				//todo optimize me:
				//push retAddress (switchskeleton+0xd)
				//call isTaskPoolRequired
				dq(PlayerCharacter__SwitchSkeleton_1406A1820 + 0xd);
			}
		};
		SetRDXtoZero d2z;
		d2z.ready();

		static SKSE::Trampoline injectedCode;
		auto injectedCodeSize = d2z.getSize() + 32; //write_branch5 allocates too much mem
		injectedCode.create(injectedCodeSize);

		injectedCode.write_branch<5>(PlayerCharacter__SwitchSkeleton_1406A1820 + offset,
		                           injectedCode.allocate(d2z));

		logger::info("injected Code @ {0:x}", PlayerCharacter__SwitchSkeleton_1406A1820 + offset);
		logger::info("fixed objects (dis)appearing during first person object animations!");

	}

	void Install()
	{
		ScrollWheelFix();
		AllowPOVchangeDuringAnimObjectAnims();
		fixAnimObjectsNotBeingParsed();
	}
}
