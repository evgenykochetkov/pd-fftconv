// uses https://github.com/thirtythreeforty/FFTConvolver,
// which is a fork of https://github.com/grawlinson/FFTConvolver,
// which is in turn forked from the original https://github.com/HiFi-LoFi/FFTConvolver

#include <m_pd.h>
#include <FFTConvolver/FFTConvolver.h>

static t_class *fftconv_class;

typedef struct _fftconv {
    t_object    x_obj;
    fftconvolver::FFTConvolver *convolver;
} t_fftconv;

static t_int *fftconv_perform(t_int *w) {
    t_fftconv *x = (t_fftconv *) (w[1]);
    int len = (int)(w[2]);    
    t_float *input = (t_float *)(w[3]);
    t_float *output = (t_float *)(w[4]);

    x->convolver->process(input, output, len);

    return w + 5;
}

static void fftconv_dsp(t_fftconv *x, t_signal **sp) {
    dsp_add(fftconv_perform, 4, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec);
}

static void fftconv_free(t_fftconv *x) {
    delete x->convolver;
}

static void fftconv_set(t_fftconv *x, t_symbol *array_name) {
    t_garray *array_ptr;
    if(!(array_ptr = (t_garray *)pd_findbyclass(array_name, garray_class))) {
        pd_error(x, "[fftconv~]: array '%s' not found", array_name->s_name);
        return;
    }

    int size;
    t_word *vec;
    if(!garray_getfloatwords(array_ptr, &size, &vec)) {
        // TODO: in Pd sources, this error is reported as "bad template". What does it mean in this context?
        pd_error(x, "[fftconv~]: can't read array '%s'", array_name->s_name);
        return;
    }

    // this formula is taken from
    // https://github.com/sdatkinson/NeuralAmpModelerPlugin/issues/100#issuecomment-1455273839
    const float gain = pow(10, -18 * 0.05);

    std::vector<t_float> ir(size, 0);
    for (int i = 0; i < size; i++) {
        ir[i] = vec[i].w_float * gain;
    }

    if (!x->convolver->init(64, ir.data(), size)) {
        pd_error(x, "[fftconv~]: failed to initialize convolver");
    }
}

static void *fftconv_new(t_symbol *, int argc, t_atom *argv) {
    t_fftconv *x = (t_fftconv *)pd_new(fftconv_class);

    outlet_new(&x->x_obj, gensym("signal"));

    x->convolver = new fftconvolver::FFTConvolver();

    if (argc && argv->a_type == A_SYMBOL) {
        t_symbol *array_name = atom_getsymbolarg(0, argc, argv);
        fftconv_set(x, array_name);
    }

    return x;
}

extern "C" void fftconv_tilde_setup(void) {
    fftconv_class = class_new(
        gensym("fftconv~"),
        (t_newmethod)fftconv_new,
        (t_method)fftconv_free,
        sizeof(t_fftconv),
        0,
        A_GIMME,
        0
    );
    class_addmethod(fftconv_class, nullfn, gensym("signal"), A_NULL);
    class_addmethod(fftconv_class, (t_method)fftconv_dsp, gensym("dsp"), A_CANT,  0);
    class_addmethod(fftconv_class, (t_method)fftconv_set, gensym("set"), A_SYMBOL, 0);
}
