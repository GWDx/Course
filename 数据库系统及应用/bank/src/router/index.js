import {
    createRouter,
    createWebHistory
} from 'vue-router'
import HomeView from '../views/HomeView.vue'

const router = createRouter({
    history: createWebHistory(
        import.meta.env.BASE_URL),
    routes: [{
            path: '/',
            name: 'home',
            component: HomeView
        },
        {
            path: '/about',
            name: 'about',
            // route level code-splitting
            // this generates a separate chunk (About.[hash].js) for this route
            // which is lazy-loaded when the route is visited.
            component: () => import('../views/AboutView.vue')
        },
        {
            path: '/admin',
            name: 'admin',
            component: () => import('../views/AdminView.vue')
        },
        {
            path: '/admin/client',
            name: 'admin-client',
            component: () => import('../views/Admin/ClientView.vue')
        },
        {
            path: '/admin/account',
            name: 'admin-account',
            component: () => import('../views/Admin/AccountView.vue')
        },
        {
            path: '/admin/loan',
            name: 'admin-loan',
            component: () => import('../views/Admin/LoanView.vue')
        },
        {
            path: '/admin/statistics',
            name: 'admin-statistics',
            component: () => import('../views/Admin/StatisticsView.vue')
        }
    ]
})

export default router
