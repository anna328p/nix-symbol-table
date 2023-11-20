#include <iostream>
#include <string>
#include <string_view>
#include <typeinfo>

#include <eval.hh>
#include <value.hh>
#include <symbol-table.hh>

using nix::EvalState, nix::Value, nix::BindingsBuilder, nix::PosIdx, nix::PrimOp;
using nix::Pos, nix::NixStringContext, nix::Symbol;

class ExtSymbol : public nix::ExternalValueBase {
	std::string s_name;
	nix::Symbol &s_sym;

public:
	ExtSymbol(nix::Symbol &new_sym, std::string_view &new_name)
			: s_sym(new_sym), s_name(new_name) { }

	auto name() const { return s_name; }
	auto sym() const { return s_sym; }

	std::ostream & print(std::ostream &stream) const {
		stream << "Symbol[" << s_name << "]";
		return stream;
	}

	std::string showType() const {
		return std::string("a symbol");
	}

	std::string typeOf() const {
		return std::string("xSymbol");
	}

	std::string coerceToString(const Pos &pos, NixStringContext &context, bool copyMore, bool copyToStore) const {
		return std::string(s_name);
	};

	bool operator==(const nix::ExternalValueBase &other) const {
		if (typeid(other) != typeid(ExtSymbol)) { return false; }

		auto &other_xsym = dynamic_cast<const ExtSymbol&>(other);
		return sym() == other_xsym.sym();
	}

	~ExtSymbol() {}
};

static void op_toSymbol(EvalState &state, const PosIdx pos, Value **args, Value &v) {
	auto name = state.forceString(*args[0], pos,
		   "while evaluating the argument passed to toSymbol");

	auto sym = state.symbols.create(name);

	v.mkExternal(new ExtSymbol(sym, name));
}

static void op_resolveSymbol(EvalState &state, const PosIdx pos, Value **args, Value &v) {
	state.forceValue(*args[0], pos);

	if (args[0]->type() != nix::nExternal \
			|| typeid(args[0]->external) != typeid(ExtSymbol)) {
		state.error("received %s, expected symbol", nix::showType(*args[0])) \
			.debugThrow<nix::TypeError>();
	}

	auto &xsym = dynamic_cast<const ExtSymbol&>(*args[0]->external);

	auto sym_str = state.symbols[xsym.sym()];

	v.mkString(sym_str);
}

extern "C" void initialize(EvalState &state, Value &v) {
	auto attrs = state.buildBindings(2);

	attrs.alloc("toSymbol").mkPrimOp(new PrimOp {
		.name = "toSymbol",
		.arity = 1,
		.fun = op_toSymbol
	});

	attrs.alloc("resolveSymbol").mkPrimOp(new PrimOp {
		.name = "resolveSymbol",
		.arity = 1,
		.fun = op_resolveSymbol
	});

	v.mkAttrs(attrs);
}
